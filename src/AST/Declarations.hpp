#pragma once

#include <sstream>
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

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool success = true;
        //     if (!_Type->Analyze(syms))
        //         success = false;
        //     for (auto &i : _VarList)
        //     {
        //         auto var = i->GetVar();
        //         if (!i->Analyze(syms))
        //         {
        //             success = false;
        //             continue;
        //         }
        //         if (!syms->AddSymbol(var->GetName(), _Type.get(), var))
        //         {
        //             std::ostringstream ss;
        //             ss << "Redeclaration of '" << var->GetName() << '\'';
        //             ErrorHandler::PrintError(ss.str(), var->GetLocation());
        //             success = false;
        //         }
        //     }
        //     return success;
        // }

        inline virtual bool CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::Module &mod, llvm::IRBuilder<> &builder) override
        {
            auto type = _Type->TypeGen(context);
            bool success = true;
            for (auto &i : _VarList)
                if (!i->CodeGen(syms, context, mod, builder, type))
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

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool success = true;
        //     if (!syms->AddSymbol(_FuncDecl->GetName(), _Type.get(), _FuncDecl.get()))
        //     {
        //         std::ostringstream ss;
        //         ss << "Redeclaration of '" << _FuncDecl->GetName() << '\'';
        //         ErrorHandler::PrintError(ss.str(), _FuncDecl->GetLocation());
        //         success = false;
        //     }
        //     auto child = syms->AddChild();
        //     if (!_FuncDecl->Analyze(child))
        //         success = false;
        //     if (!_Body->Analyze(child))
        //         success = false;
        //     return success;
        // }

        inline virtual bool CodeGen(SymbolTable &syms, llvm::LLVMContext &context,
                                    llvm::Module &mod, llvm::IRBuilder<> &builder) override
        {
            // TODO
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

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool success = true;
        //     for (auto &i : _DeclList)
        //         if (!i->Analyze(syms))
        //             success = false;
        //     return success;
        // }

        inline bool CodeGen(llvm::LLVMContext &context, llvm::Module &mod)
        {
            SymbolTable syms;
            llvm::FunctionType *entryFT = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
            llvm::Function *entryF = llvm::Function::Create(entryFT, llvm::Function::ExternalLinkage, "entry", &mod);
            llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "entry", entryF);
            llvm::IRBuilder<> builder(context);
            builder.SetInsertPoint(entryBB);

            bool success = true;
            for (auto &i : _DeclList)
                if (!i->CodeGen(syms, context, mod, builder))
                    success = false;

            builder.CreateRet(nullptr);
            return success;

            // TODO: should I put 'entry' into symbol table?
        }
    };
} // namespace ast