#pragma once

#include "Base.hpp"
#include "Expressions.hpp"

namespace ast
{
    class Statement : public Base
    {
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
    };

    class ReturnStmt : public Statement
    {
    private:
        ptr<Expression> _Expr;

    public:
        inline explicit ReturnStmt() = default;
        inline explicit ReturnStmt(ptr<Base> &expr) : _Expr(to<Expression>(expr)) {}

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
    };

    class StatementList : public Statement
    {
    private:
        arr<ptr<Statement>> _StatementList;

    public:
        inline void AddStmt(ptr<Base> &stmt) { _StatementList.push_back(to<Statement>(stmt)); }

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
    };
} // namespace ast