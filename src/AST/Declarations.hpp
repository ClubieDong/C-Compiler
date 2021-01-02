#pragma once

#include <sstream>
#include "Base.hpp"
#include "Expressions.hpp"
#include "Statements.hpp"

namespace ast
{
    class TypePrimitive : public Base
    {
    };

    class BasicType : public TypePrimitive
    {
    public:
        enum Type
        {
            VOID,
            INT,
            BOOL,
        };

    private:
        Type _Type;

    public:
        inline explicit BasicType(Type type) : _Type(type) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"VOID", "INT", "BOOL"};
            os << hint << "BasicType: " << ENUM_NAMES[_Type] << '\n';
        }
    };

    class CustomType : public TypePrimitive
    {
    private:
        ptr<ID> _Name;

    public:
        inline explicit CustomType(ptr<Base> &name) : _Name(to<ID>(name)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "CustomType: \n";
            _Name->Show(os, hint + '\t');
        }
    };

    class Decl : public Base
    {
    public:
        inline virtual std::string GetName() const = 0;
        inline virtual const ErrorHandler::Location &GetLocation() const = 0;
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

        inline virtual std::string GetName() const { return _Name->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const { return _Name->GetLocation(); }
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

        inline virtual bool Analyze(SymbolTable *syms)
        {
            bool success = true;
            for (auto& [type, decl] : _ParamList)
                if (!syms->AddSymbol(decl->GetName(), type.get(), decl.get()))
                {
                    std::ostringstream ss;
                    ss << "Redeclaration of '" << decl->GetName() << '\'';
                    ErrorHandler::PrintError(ss.str(), decl->GetLocation());
                    success = false;
                }
            return success;
        }

        inline virtual std::string GetName() const { return _Name->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const { return _Name->GetLocation(); }
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

        inline virtual bool Analyze(SymbolTable *syms)
        {
            return _Type->Analyze(syms);
        }

        inline virtual std::string GetName() const { return _Type->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const { return _Type->GetLocation(); }
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

        inline virtual bool Analyze(SymbolTable *syms)
        {
            return _Type->Analyze(syms);
        }

        inline virtual std::string GetName() const { return _Type->GetName(); };
        inline virtual const ErrorHandler::Location &GetLocation() const { return _Type->GetLocation(); }
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

        inline virtual bool Analyze(SymbolTable *syms)
        {
            bool success = true;
            if (!_Var->Analyze(syms))
                success = false;
            if (_Init && !_Init->Analyze(syms))
                success = false;
            return success;
        }

        inline Decl *GetVar() { return _Var.get(); }
    };

    class Declaration : public Statement
    {
    };

    class VarDeclaration : public Declaration
    {
    private:
        ptr<TypePrimitive> _Type;
        arr<ptr<InitDecl>> _VarList;

    public:
        inline explicit VarDeclaration(ptr<Base> &var) { _VarList.push_back(to<InitDecl>(var)); }

        inline void AddVar(ptr<Base> &var) { _VarList.push_back(to<InitDecl>(var)); }
        inline void SetType(ptr<Base> &type) { _Type = to<TypePrimitive>(type); }

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "VarDeclaration: \n";
            os << hint << "\tType: \n";
            _Type->Show(os, hint + "\t\t");
            for (const auto &var : _VarList)
            {
                os << hint << "\tVar: \n";
                var->Show(os, hint + "\t\t");
            }
        }

        inline virtual bool Analyze(SymbolTable *syms)
        {
            bool success = true;
            if (!_Type->Analyze(syms))
                success = false;
            for (auto &i : _VarList)
            {
                auto var = i->GetVar();
                if (!i->Analyze(syms))
                {
                    success = false;
                    continue;
                }
                if (!syms->AddSymbol(var->GetName(), _Type.get(), var))
                {
                    std::ostringstream ss;
                    ss << "Redeclaration of '" << var->GetName() << '\'';
                    ErrorHandler::PrintError(ss.str(), var->GetLocation());
                    success = false;
                }
            }
            return success;
        }
    };

    class FuncDeclaration : public Declaration
    {
    private:
        ptr<TypePrimitive> _Type;
        ptr<Decl> _FuncDecl;
        ptr<StatementList> _Body;

    public:
        inline explicit FuncDeclaration(ptr<Base> &type, ptr<Base> &funcDecl, ptr<Base> &body)
            : _Type(to<TypePrimitive>(type)), _FuncDecl(to<Decl>(funcDecl)), _Body(to<StatementList>(body)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "FuncDeclaration: \n";
            os << hint << "\tType: \n";
            _Type->Show(os, hint + "\t\t");
            os << hint << "\tFuncDecl: \n";
            _FuncDecl->Show(os, hint + "\t\t");
            os << hint << "\tBody: \n";
            _Body->Show(os, hint + "\t\t");
        }

        inline virtual bool Analyze(SymbolTable *syms)
        {
            bool success = true;
            if (!syms->AddSymbol(_FuncDecl->GetName(), _Type.get(), _FuncDecl.get()))
            {
                std::ostringstream ss;
                ss << "Redeclaration of '" << _FuncDecl->GetName() << '\'';
                ErrorHandler::PrintError(ss.str(), _FuncDecl->GetLocation());
                success = false;
            }
            auto child = syms->AddChild();
            if (!_FuncDecl->Analyze(child))
                success = false;
            if (!_Body->Analyze(child))
                success = false;
            return success;
        }
    };

    class DeclarationList : public Base
    {
    private:
        arr<ptr<Declaration>> _DeclList;

    public:
        inline explicit DeclarationList(ptr<Base> &decl) { _DeclList.push_back(to<Declaration>(decl)); }

        inline void AddDecl(ptr<Base> &decl) { _DeclList.push_back(to<Declaration>(decl)); }

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            for (const auto &decl : _DeclList)
            {
                os << hint << "Declaration: \n";
                decl->Show(os, hint + '\t');
            }
        }

        inline virtual bool Analyze(SymbolTable *syms)
        {
            bool success = true;
            for (auto &i : _DeclList)
                if (!i->Analyze(syms))
                    success = false;
            return success;
        }
    };
} // namespace ast