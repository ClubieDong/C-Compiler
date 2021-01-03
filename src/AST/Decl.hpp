#pragma once

#include <sstream>
#include "Base.hpp"
#include "Expressions.hpp"
#include "Statements.hpp"
#include "TypePrimitive.hpp"

namespace ast
{
    class FuncDecl;
    class ReferenceDecl;

    class Decl : public Base
    {
    public:
        inline virtual std::string GetName() const = 0;
        inline virtual const ErrorHandler::Location &GetLocation() const = 0;
        inline virtual FuncDecl *GetFuncDecl() = 0;

        inline virtual llvm::Type *TypeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                           llvm::IRBuilder<> &builder, llvm::Type *type, bool isRef) = 0;
    };

    class VarDecl : public Decl
    {
    private:
        ptr<ID> _Name;

    public:
        inline explicit VarDecl() = default;
        inline explicit VarDecl(ptr<Base> &name) : _Name(to<ID>(name)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "VarDecl: \n";
            _Name->Show(os, hint + '\t');
        }

        inline virtual std::string GetName() const override { return _Name->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Name->GetLocation(); }
        inline virtual FuncDecl *GetFuncDecl() override { return nullptr; }

        inline virtual llvm::Type *TypeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                           llvm::IRBuilder<> &builder, llvm::Type *type, bool isRef) override
        {
            return type;
        }
    };

    class FuncDecl : public Decl
    {
    private:
        struct Param
        {
            ptr<TypePrimitive> _Type;
            ptr<Decl> _Decl;
            inline explicit Param(ptr<Base> &type, ptr<Base> &decl)
                : _Type(to<TypePrimitive>(type)), _Decl(to<Decl>(decl)) {}
        };

        ptr<ID> _Name;
        arr<Param> _ParamList;

    public:
        inline explicit FuncDecl() = default;
        inline explicit FuncDecl(ptr<Base> &type, ptr<Base> &decl) { _ParamList.emplace_back(type, decl); }

        inline void AddParam(ptr<Base> &type, ptr<Base> &decl) { _ParamList.emplace_back(type, decl); }
        inline void SetName(ptr<Base> &name) { _Name = to<ID>(name); }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "FuncDecl: \n";
            _Name->Show(os, hint + '\t');
            for (const auto &param : _ParamList)
            {
                os << hint << "\tParam: \n";
                os << hint << "\t\tType: \n";
                param._Type->Show(os, hint + "\t\t\t");
                os << hint << "\t\tDecl: \n";
                param._Decl->Show(os, hint + "\t\t\t");
            }
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool success = true;
        //     for (auto &[type, decl] : _ParamList)
        //         if (!syms->AddSymbol(decl->GetName(), type.get(), decl.get()))
        //         {
        //             std::ostringstream ss;
        //             ss << "Redeclaration of '" << decl->GetName() << '\'';
        //             ErrorHandler::PrintError(ss.str(), decl->GetLocation());
        //             success = false;
        //         }
        //     return success;
        // }

        inline virtual std::string GetName() const override { return _Name->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Name->GetLocation(); }
        inline virtual FuncDecl *GetFuncDecl() override { return this; }

        inline virtual llvm::Type *TypeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                           llvm::IRBuilder<> &builder, llvm::Type *type, bool isRef) override
        {
            return type;
        }
    };

    class ArrayDecl : public Decl
    {
    private:
        ptr<Decl> _Type;
        ptr<Expression> _Size;

    public:
        inline explicit ArrayDecl(ptr<Base> &type) : _Type(to<Decl>(type)) {}
        inline explicit ArrayDecl(ptr<Base> &type, ptr<Base> &size)
            : _Type(to<Decl>(type)), _Size(to<Expression>(size)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "ArrayDecl: \n";
            if (_Size)
            {
                os << hint << "\tSize: \n";
                _Size->Show(os, hint + "\t\t");
            }
            os << hint << "\tType: \n";
            _Type->Show(os, hint + "\t\t");
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     return _Type->Analyze(syms);
        // }

        inline virtual std::string GetName() const override { return _Type->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Type->GetLocation(); }
        inline virtual FuncDecl *GetFuncDecl() override { return _Type->GetFuncDecl(); }

        virtual llvm::Type *TypeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::IRBuilder<> &builder, llvm::Type *type, bool isRef) override
        {
            if (isRef)
            {
                ErrorHandler::PrintError("Array of reference is not allowed", _Type->GetLocation());
                return nullptr;
            }
            auto size = _Size->CodeGen(syms, context, builder);
            if (!size)
                return nullptr;
            if (auto c = llvm::dyn_cast<llvm::ConstantInt>(size->Value))
            {
                type = llvm::ArrayType::get(type, c->getSExtValue());
                return _Type->TypeGen(syms, context, builder, type, false);
            }
            return nullptr;
        }
    };

    class PointerDecl : public Decl
    {
    private:
        ptr<Decl> _Type;

    public:
        inline explicit PointerDecl(ptr<Base> &type) : _Type(to<Decl>(type)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "Pointer: \n";
            _Type->Show(os, hint + '\t');
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     return _Type->Analyze(syms);
        // }

        inline virtual std::string GetName() const override { return _Type->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Type->GetLocation(); }
        inline virtual FuncDecl *GetFuncDecl() override { return _Type->GetFuncDecl(); }

        virtual llvm::Type *TypeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::IRBuilder<> &builder, llvm::Type *type, bool isRef) override
        {
            if (isRef)
            {
                ErrorHandler::PrintError("Pointer to reference is not allowed", _Type->GetLocation());
                return nullptr;
            }
            type = llvm::PointerType::get(type, 0);
            return _Type->TypeGen(syms, context, builder, type, false);
        }
    };

    class ReferenceDecl : public Decl
    {
    private:
        ptr<Decl> _Type;

    public:
        inline explicit ReferenceDecl(ptr<Base> &type) : _Type(to<Decl>(type)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "Reference: \n";
            _Type->Show(os, hint + '\t');
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     return _Type->Analyze(syms);
        // }

        inline virtual std::string GetName() const override { return _Type->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const override { return _Type->GetLocation(); }
        inline virtual FuncDecl *GetFuncDecl() override { return _Type->GetFuncDecl(); }

        inline virtual llvm::Type *TypeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                           llvm::IRBuilder<> &builder, llvm::Type *type, bool isRef) override
        {
            if (isRef)
            {
                ErrorHandler::PrintError("Reference to reference is not allowed", _Type->GetLocation());
                return nullptr;
            }
            type = llvm::PointerType::get(type, 0);
            return _Type->TypeGen(syms, context, builder, type, true);
        }
    };

    class InitDecl : public Base
    {
    private:
        ptr<Decl> _Var;
        ptr<Expression> _Init;

    public:
        inline explicit InitDecl(ptr<Base> &var) : _Var(to<Decl>(var)) {}
        inline explicit InitDecl(ptr<Base> &var, ptr<Base> &init)
            : _Var(to<Decl>(var)), _Init(to<Expression>(init)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "InitDecl: \n";
            os << hint << "\tVar: \n";
            _Var->Show(os, hint + "\t\t");
            if (_Init)
            {
                os << hint << "\tInit: \n";
                _Init->Show(os, hint + "\t\t");
            }
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool success = true;
        //     if (!_Var->Analyze(syms))
        //         success = false;
        //     if (_Init && !_Init->Analyze(syms))
        //         success = false;
        //     return success;
        // }

        inline Decl *GetVar() { return _Var.get(); }

        inline llvm::Value *CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::Module &mod, llvm::IRBuilder<> &builder, llvm::Type *type)
        {
            const auto &name = _Var->GetName();
            if (!syms.TryAddSymbol(name))
            {
                std::ostringstream ss;
                ss << "Redeclaration of '" << name << '\'';
                ErrorHandler::PrintError(ss.str(), _Var->GetLocation());
                return nullptr;
            }
            bool isRef = dynamic_cast<ReferenceDecl *>(_Var.get());
            if (isRef && !_Init)
            {
                ErrorHandler::PrintError("Reference requires an initializer", _Var->GetLocation());
                return nullptr;
            }
            type = _Var->TypeGen(syms, context, builder, type, false);
            llvm::Value *value;
            if (auto func = _Var->GetFuncDecl())
            {
                // TODO: Generate function
                throw "Not implemented";
            }
            else
            {
                if (type->isVoidTy())
                {
                    ErrorHandler::PrintError("Void type is not allowed here", _Var->GetLocation());
                    return nullptr;
                }
                value = new llvm::GlobalVariable(mod, type, false,
                                                 llvm::GlobalValue::LinkageTypes::PrivateLinkage,
                                                 llvm::Constant::getNullValue(type), name);
            }
            syms.AddSymbol(_Var->GetName(), SymbolTable::Symbol(value, true));
            if (!_Init)
                return value;
            auto initValue = _Init->CodeGen(syms, context, builder);
            if (!initValue)
                return nullptr;
            if (isRef)
            {
                if (!initValue->IsRef)
                {
                    ErrorHandler::PrintError("Initial value of reference must be an lvalue", _Var->GetLocation());
                    return nullptr;
                }
                if (type != initValue->Value->getType())
                {
                    ErrorHandler::PrintError("Type of the initial value does not match the declared type", _Var->GetLocation());
                    return nullptr;
                }
                // Dereference
                initValue->IsRef = false;
                Expression::Assign(SymbolTable::Symbol(value, true), *initValue, _Var->GetLocation(), builder);
            }
            else
                Expression::Assign(SymbolTable::Symbol(value, true), *initValue, _Var->GetLocation(), builder);
            return value;
        }
    };
} // namespace ast
