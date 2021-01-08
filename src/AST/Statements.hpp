#pragma once

#include "Base.hpp"
#include "Expressions.hpp"

namespace ast
{
    class Statement : public Base
    {
    public:
        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder)
        {
            return true;
        }
    };

    class ExprStmt : public Statement
    {
    private:
        ptr<Expression> _Expr;

    public:
        inline explicit ExprStmt() = default;
        inline explicit ExprStmt(ptr<Base> &expr) : _Expr(to<Expression>(expr)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (_Expr)
                _Expr->Show(os, hint);
            else
                os << hint << "Empty expression statement" << '\n';
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     if (_Expr)
        //         return _Expr->Analyze(syms);
        //     return true;
        // }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            if (!_Expr)
                return true;
            if (_Expr->CodeGen(syms, context, builder))
                return true;
            return false;
        }
    };

    class IfStmt : public Statement
    {
    private:
        ptr<Expression> _CondExpr;
        ptr<Statement> _Then, _Else;

    public:
        inline explicit IfStmt(ptr<Base> &cond, ptr<Base> &then)
            : _CondExpr(to<Expression>(cond)), _Then(to<Statement>(then)) {}
        inline explicit IfStmt(ptr<Base> &cond, ptr<Base> &then, ptr<Base> &else_)
            : _CondExpr(to<Expression>(cond)), _Then(to<Statement>(then)), _Else(to<Statement>(else_)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "IfStmt\n";
            os << hint << "\tCondition: \n";
            _CondExpr->Show(os, hint + "\t\t");
            os << hint << "\tThen: \n";
            _Then->Show(os, hint + "\t\t");
            if (_Else)
            {
                os << hint << "\tElse: \n";
                _Else->Show(os, hint + "\t\t");
            }
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool c = _CondExpr->Analyze(syms);
        //     bool t = _Then->Analyze(syms);
        //     if (_Else)
        //     {
        //         bool e = _Else->Analyze(syms);
        //         return c && t && e;
        //     }
        //     return c && t;
        // }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            auto func = builder.GetInsertBlock()->getParent();

            auto thenBB = llvm::BasicBlock::Create(context, "if.then", func, 0);
            auto elseBB = llvm::BasicBlock::Create(context, "if.else", func, 0);
            auto mergeBB = llvm::BasicBlock::Create(context, "if.merge", func, 0);

            auto condValue = _CondExpr->CodeGen(syms, context, builder);
            if (!condValue)
                return false;
            condValue = Expression::CastLLVMType(*condValue, llvm::Type::getInt1Ty(context), false, _CondExpr->GetLocation(), builder);
            if (!condValue)
                return false;
            builder.CreateCondBr(condValue->Value, thenBB, elseBB);

            builder.SetInsertPoint(thenBB);
            if (!_Then->StmtGen(syms, context, builder))
                return false;
            builder.CreateBr(mergeBB);

            builder.SetInsertPoint(elseBB);
            if (_Else && !_Else->StmtGen(syms, context, builder))
                return false;
            builder.CreateBr(mergeBB);

            builder.SetInsertPoint(mergeBB);
            return true;
        }
    };

    class WhileStmt : public Statement
    {
    private:
        ptr<Expression> _CondExpr;
        ptr<Statement> _Body;

    public:
        inline explicit WhileStmt(ptr<Base> &cond, ptr<Base> &body)
            : _CondExpr(to<Expression>(cond)), _Body(to<Statement>(body)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "WhileStmt: \n";
            os << hint << "\tCondition: \n";
            _CondExpr->Show(os, hint + "\t\t");
            os << hint << "\tLoop body: \n";
            _Body->Show(os, hint + "\t\t");
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool c = _CondExpr->Analyze(syms);
        //     bool b = _Body->Analyze(syms);
        //     return c && b;
        // }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            auto func = builder.GetInsertBlock()->getParent();

            auto condBB = llvm::BasicBlock::Create(context, "while.cond", func, 0);
            auto bodyBB = llvm::BasicBlock::Create(context, "while.body", func, 0);
            auto mergeBB = llvm::BasicBlock::Create(context, "while.merge", func, 0);

            builder.CreateBr(condBB);

            builder.SetInsertPoint(condBB);
            auto condValue = _CondExpr->CodeGen(syms, context, builder);
            if (!condValue)
                return false;
            condValue = Expression::CastLLVMType(*condValue, llvm::Type::getInt1Ty(context), false, _CondExpr->GetLocation(), builder);
            if (!condValue)
                return false;
            builder.CreateCondBr(condValue->Value, bodyBB, mergeBB);

            builder.SetInsertPoint(bodyBB);
            if (!_Body->StmtGen(syms, context, builder))
                return false;
            builder.CreateBr(condBB);

            builder.SetInsertPoint(mergeBB);
            return true;
        }
    };

    class ForStmt : public Statement
    {
    private:
        ptr<Statement> _Init;
        ptr<Expression> _Cond;
        ptr<Expression> _Incre;
        ptr<Statement> _Body;

    public:
        inline explicit ForStmt(ptr<Base> &init, ptr<Base> &cond, ptr<Base> &incre, ptr<Base> &body)
            : _Init(to<Statement>(init)), _Cond(to<Expression>(cond)),
              _Incre(to<Expression>(incre)), _Body(to<Statement>(body)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "ForStmt: \n";
            os << hint << "\tInit statement: \n";
            _Init->Show(os, hint + "\t\t");
            os << hint << "\tCondition: \n";
            _Cond->Show(os, hint + "\t\t");
            os << hint << "\tIncrement statement: \n";
            _Incre->Show(os, hint + "\t\t");
            os << hint << "\tLoop body: \n";
            _Body->Show(os, hint + "\t\t");
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     bool c = _CondExpr->Analyze(syms);
        //     bool b = _Body->Analyze(syms);
        //     return c && b;
        // }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            auto func = builder.GetInsertBlock()->getParent();

            auto condBB = llvm::BasicBlock::Create(context, "for.cond", func, 0);
            auto bodyBB = llvm::BasicBlock::Create(context, "for.body", func, 0);
            auto mergeBB = llvm::BasicBlock::Create(context, "for.merge", func, 0);

            if (!_Init->StmtGen(syms, context, builder))
                return false;
            builder.CreateBr(condBB);

            builder.SetInsertPoint(condBB);
            auto condValue = _Cond->CodeGen(syms, context, builder);
            if (!condValue)
                return false;
            condValue = Expression::CastLLVMType(*condValue, llvm::Type::getInt1Ty(context), false, _Cond->GetLocation(), builder);
            if (!condValue)
                return false;
            builder.CreateCondBr(condValue->Value, bodyBB, mergeBB);

            builder.SetInsertPoint(bodyBB);
            if (!_Body->StmtGen(syms, context, builder))
                return false;
            if (!_Incre->CodeGen(syms, context, builder))
                return false;
            builder.CreateBr(condBB);

            builder.SetInsertPoint(mergeBB);
            return true;
        }
    };

    class ReturnStmt : public Statement
    {
    private:
        ptr<Expression> _Expr;
        ErrorHandler::Location _Location;

    public:
        inline explicit ReturnStmt(const ErrorHandler::Location &loc) : _Location(loc){};
        inline explicit ReturnStmt(ptr<Base> &expr, const ErrorHandler::Location &loc)
            : _Expr(to<Expression>(expr)), _Location(loc) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (_Expr)
            {
                os << hint << "ReturnStmt: \n";
                _Expr->Show(os, hint + '\t');
            }
            else
                os << hint << "ReturnStmt\n";
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     if (_Expr)
        //         return _Expr->Analyze(syms);
        //     return true;
        // }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            auto func = builder.GetInsertBlock()->getParent();

            if (!_Expr && func->getReturnType()->isVoidTy())
            {
                builder.CreateRet(nullptr);
                return true;
            }
            if (_Expr && func->getReturnType()->isVoidTy())
            {
                ErrorHandler::PrintError("Return type should be void", _Location);
                return false;
            }
            if (!_Expr && !func->getReturnType()->isVoidTy())
            {
                ErrorHandler::PrintError("Return type should not be void", _Location);
                return false;
            }
            auto retValue = _Expr->CodeGen(syms, context, builder);
            if (!retValue)
                return false;
            retValue = Expression::CastLLVMType(*retValue, func->getReturnType(), false, _Location, builder);
            if (!retValue)
                return false;

            auto retBB = llvm::BasicBlock::Create(context, "return.after", func, 0);
            builder.CreateRet(retValue->Value);

            builder.SetInsertPoint(retBB);
            return true;
        }
    };

    class StatementList : public Statement
    {
    private:
        arr<ptr<Statement>> _StatementList;

    public:
        inline void AddStmt(ptr<Base> &stmt)
        {
            _StatementList.push_back(to<Statement>(stmt));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (_StatementList.empty())
                os << hint << "No Statements\n";
            else
            {
                os << hint << "StatementList: \n";
                for (const auto &stmt : _StatementList)
                    stmt->Show(os, hint + '\t');
            }
        }

        // inline virtual bool Analyze(SymbolTable *syms)
        // {
        //     auto child = syms->AddChild();
        //     bool success = true;
        //     for (auto& i : _StatementList)
        //         if (!i->Analyze(child))
        //             success = false;
        //     return success;
        // }

        inline virtual bool StmtGen(SymbolTable &syms, llvm::LLVMContext &context, llvm::IRBuilder<> &builder) override
        {
            bool success = true;
            auto child = syms.AddChild();
            for (auto &i : _StatementList)
                if (!i->StmtGen(*child, context, builder))
                    success = false;
            return success;
        }
    };
} // namespace ast