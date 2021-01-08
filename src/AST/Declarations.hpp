#pragma once

#include <sstream>
#include <vector>
#include "Base.hpp"
#include "Expressions.hpp"
#include "Statements.hpp"
#include "TypePrimitive.hpp"
#include "Decl.hpp"

namespace ast
{
    class Declaration : public Statement
    {
    public:
        inline virtual bool CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::Module &mod, llvm::IRBuilder<> &builder) = 0;
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

        inline virtual bool CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::Module &mod, llvm::IRBuilder<> &builder) override
        {
            auto type = _Type->TypeGen(context);
            bool success = true;
            for (auto &i : _VarList)
                if (!i->CodeGen(syms, context, builder, type, &mod))
                    success = false;
            return success;
        }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            auto type = _Type->TypeGen(context);
            bool success = true;
            for (auto &i : _VarList)
                if (!i->CodeGen(syms, context, builder, type, nullptr))
                    success = false;
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

        inline virtual bool CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::Module &mod, llvm::IRBuilder<> &builder) override
        {
            auto name = _FuncDecl->GetName();
            if (name == "main")
                name = "__main__";
            // TODO: Check return type and parameters of main function
            // but it seems to work well with arbitary return type and parameters of main function
            if (!syms.TryAddSymbol(name))
            {
                std::ostringstream ss;
                ss << "Redeclaration of '" << name << '\'';
                ErrorHandler::PrintError(ss.str(), _FuncDecl->GetLocation());
                return false;
            }
            auto retTy = _Type->TypeGen(context);
            retTy = _FuncDecl->TypeGen(syms, context, builder, retTy, false);
            auto func = _FuncDecl->GetFuncDecl();
            assert(func);
            auto ft = func->FunctionTypeGen(syms, context, builder, retTy);
            if (!ft)
                return false;
            auto *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, mod);
            syms.AddSymbol(name, f);
            auto *entryBB = llvm::BasicBlock::Create(context, "entry", f);
            llvm::IRBuilder<> bbBuilder(context);
            bbBuilder.SetInsertPoint(entryBB);
            auto child = syms.AddChild();
            if (!func->SetParamName(*child, f, bbBuilder))
                return false;
            if (!_Body->StmtGen(*child, context, bbBuilder))
                return false;
            if (retTy->isVoidTy())
                bbBuilder.CreateRet(nullptr);
            else
                bbBuilder.CreateRet(llvm::Constant::getNullValue(retTy));
            return true;
        }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            ErrorHandler::PrintError("Nested function is not allowed", _FuncDecl->GetLocation());
            return false;
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

        inline bool CodeGen(llvm::LLVMContext &context, llvm::Module &mod)
        {
            SymbolTable syms;
            llvm::FunctionType *entryFT = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
            llvm::Function *entryF = llvm::Function::Create(entryFT, llvm::Function::ExternalLinkage, "main", &mod);
            llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "entry", entryF);
            llvm::IRBuilder<> builder(context);
            builder.SetInsertPoint(entryBB);

            bool success = true;
            for (auto &i : _DeclList)
                if (!i->CodeGen(syms, context, mod, builder))
                    success = false;

            auto func = mod.getFunction("__main__");
            if (!func)
            {
                ErrorHandler::PrintError("Expect main function", ErrorHandler::Location());
                return false;
            }
            std::vector<llvm::Value *> args;
            for (unsigned int i = 0; i < func->arg_size(); ++i)
            {
                auto t = func->getArg(i)->getType();
                auto c = llvm::Constant::getNullValue(t);
                args.push_back(c);
            }
            builder.CreateCall(func, args);
            builder.CreateRet(nullptr);
            return success;
        }
    };
} // namespace ast
