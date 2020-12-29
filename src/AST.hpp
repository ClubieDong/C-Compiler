#pragma once

#include <memory>
#include <vector>
#include <cassert>
#include <iostream>
#include "Utilities.hpp"

namespace ast
{
    template <typename T>
    using ptr = std::unique_ptr<T>;

    template <typename T>
    using arr = std::vector<T>;

    class Base
    {
    public:
        virtual ~Base() = default;

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const = 0;
        inline friend std::ostream &operator<<(std::ostream &os, const Base &obj)
        {
            obj.Show(os);
            return os;
        }
    };

    class Expression : public Base
    {
    };

    class Constant : public Expression
    {
    private:
        int _Value;

    public:
        inline explicit Constant(int value) : _Value(value) {}
        inline static ptr<Base> Create(int value)
        {
            return std::make_unique<Constant>(value);
        }

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "Constant: " << _Value << '\n';
        }
    };

    class Variable : public Expression
    {
    private:
        std::string _ID;

    public:
        inline explicit Variable(const std::string &id) : _ID(id) {}
        inline static ptr<Base> Create(const std::string &id)
        {
            return std::make_unique<Variable>(id);
        }

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "Variable: " << _ID << '\n';
        }
    };

    class BiOpExpr : public Expression
    {
    public:
        enum BiOp
        {
            ASSIGN,
            LESS,
            GREATER,
            LESS_EQUAL,
            GREATER_EQUAL,
            EQUAL,
            NOT_EQUAL,
            ADD,
            SUB,
            MUL,
            DIV,
        };

    private:
        ptr<Expression> _Left, _Right;
        BiOp _Op;

    public:
        inline explicit BiOpExpr(ptr<Expression> left, ptr<Expression> right, BiOp op)
            : _Left(std::move(left)), _Right(std::move(right)), _Op(op) {}
        inline static ptr<Base> Create(ptr<Base> left, ptr<Base> right, BiOp op)
        {
            auto leftExpr = dynamic_pointer_cast<Expression>(left);
            assert(leftExpr);
            auto rightExpr = dynamic_pointer_cast<Expression>(right);
            assert(rightExpr);
            return std::make_unique<BiOpExpr>(std::move(leftExpr), std::move(rightExpr), op);
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"ASSIGN", "LESS", "GREATER", "LESS_EQUAL", "GREATER_EQUAL",
                                                  "EQUAL", "NOT_EQUAL", "ADD", "SUB", "MUL", "DIV"};
            os << hint << "Operator: " << ENUM_NAMES[_Op] << '\n';
            os << hint << "Left: " << '\n';
            _Left->Show(os, hint + '\t');
            os << hint << "Right: " << '\n';
            _Right->Show(os, hint + '\t');
        }
    };

    class UnOpExpr : public Expression
    {
    public:
        enum UnOp
        {
            NEG
        };

    private:
        ptr<Expression> _Operand;
        UnOp _Op;

    public:
        inline explicit UnOpExpr(ptr<Expression> operand, UnOp op)
            : _Operand(std::move(operand)), _Op(op) {}
        inline static ptr<Base> Create(ptr<Base> operand, UnOp op)
        {
            auto operandExpr = dynamic_pointer_cast<Expression>(operand);
            assert(operandExpr);
            return std::make_unique<UnOpExpr>(std::move(operandExpr), op);
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"NEG"};
            os << hint << "Operator: " << ENUM_NAMES[_Op] << '\n';
            os << hint << "Operand: " << '\n';
            _Operand->Show(os, hint + '\t');
        }
    };

    class ArgList : public Base
    {
        friend class CallExpr;

    private:
        arr<ptr<Expression>> _ArgList;

    public:
        inline explicit ArgList() = default;
        inline explicit ArgList(ptr<Expression> expr) { _ArgList.push_back(std::move(expr)); }
        inline static ptr<Base> Create() { return std::make_unique<ArgList>(); }
        inline static ptr<Base> Create(ptr<Base> expr)
        {
            auto exprExpr = dynamic_pointer_cast<Expression>(expr);
            assert(exprExpr);
            return std::make_unique<ArgList>(std::move(exprExpr));
        }
        inline void Add(ptr<Base> expr)
        {
            auto exprExpr = dynamic_pointer_cast<Expression>(expr);
            assert(exprExpr);
            _ArgList.push_back(std::move(exprExpr));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (_ArgList.empty())
                os << hint << "No Arguments" << '\n';
            else
                for (const auto &expr : _ArgList)
                {
                    os << hint << "Argument: " << '\n';
                    expr->Show(os, hint + '\t');
                }
        }
    };

    class CallExpr : public Expression
    {
    private:
        ptr<Expression> _Func;
        ptr<ArgList> _ArgList;

    public:
        inline explicit CallExpr(ptr<Expression> func, ptr<ArgList> argList)
            : _Func(std::move(func)), _ArgList(std::move(argList)) {}
        inline static ptr<Base> Create(ptr<Base> func, ptr<Base> argList)
        {
            auto funcExpr = dynamic_pointer_cast<Expression>(func);
            assert(funcExpr);
            auto args = dynamic_pointer_cast<ArgList>(argList);
            assert(args);
            return std::make_unique<CallExpr>(std::move(funcExpr), std::move(args));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "Function: " << '\n';
            _Func->Show(os, hint + '\t');
            os << hint << "Arguments: " << '\n';
            _ArgList->Show(os, hint + '\t');
        }
    };

    class Statement : public Base
    {
    };

    class ExprStmt : public Statement
    {
    private:
        ptr<Expression> _Expr;

    public:
        inline explicit ExprStmt(ptr<Expression> expr = nullptr) : _Expr(std::move(expr)) {}
        inline static ptr<Base> Create(ptr<Base> expr = nullptr)
        {
            auto exprStmt = dynamic_pointer_cast<Expression>(expr);
            return std::make_unique<ExprStmt>(std::move(exprStmt));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (_Expr)
            {
                os << hint << "Expression: " << '\n';
                _Expr->Show(os, hint + '\t');
            }
            else
                os << hint << "Empty expression statement" << '\n';
        }
    };

    class IfStmt : public Statement
    {
    private:
        ptr<Expression> _CondExpr;
        ptr<Statement> _Then, _Else;

    public:
        inline explicit IfStmt(ptr<Expression> cond, ptr<Statement> then, ptr<Statement> else_ = nullptr)
            : _CondExpr(std::move(cond)), _Then(std::move(then)), _Else(std::move(else_)) {}
        inline static ptr<Base> Create(ptr<Base> cond, ptr<Base> then, ptr<Base> else_ = nullptr)
        {
            auto condExpr = dynamic_pointer_cast<Expression>(cond);
            assert(condExpr);
            auto thenStmt = dynamic_pointer_cast<Statement>(then);
            assert(thenStmt);
            auto elseStmt = dynamic_pointer_cast<Statement>(else_);
            return std::make_unique<IfStmt>(std::move(condExpr), std::move(thenStmt), std::move(elseStmt));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "Condition: " << '\n';
            _CondExpr->Show(os, hint + '\t');
            os << hint << "Then: " << '\n';
            _Then->Show(os, hint + '\t');
            if (_Else)
            {
                os << hint << "Else: " << '\n';
                _Else->Show(os, hint + '\t');
            }
        }
    };

    class WhileStmt : public Statement
    {
    private:
        ptr<Expression> _CondExpr;
        ptr<Statement> _Body;

    public:
        inline explicit WhileStmt(ptr<Expression> cond, ptr<Statement> body)
            : _CondExpr(std::move(cond)), _Body(std::move(body)) {}
        inline static ptr<Base> Create(ptr<Base> cond, ptr<Base> body)
        {
            auto condExpr = dynamic_pointer_cast<Expression>(cond);
            assert(condExpr);
            auto bodyStmt = dynamic_pointer_cast<Statement>(body);
            assert(bodyStmt);
            return std::make_unique<WhileStmt>(std::move(condExpr), std::move(bodyStmt));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "Condition: " << '\n';
            _CondExpr->Show(os, hint + '\t');
            os << hint << "Loop body: " << '\n';
            _Body->Show(os, hint + '\t');
        }
    };

    class ReturnStmt : public Statement
    {
    private:
        ptr<Expression> _Expr;

    public:
        inline explicit ReturnStmt(ptr<Expression> expr = nullptr) : _Expr(std::move(expr)) {}
        inline static ptr<Base> Create(ptr<Base> expr = nullptr)
        {
            auto exprExpr = dynamic_pointer_cast<Expression>(expr);
            return std::make_unique<ReturnStmt>(std::move(exprExpr));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (_Expr)
            {
                os << hint << "Return expression: " << '\n';
                _Expr->Show(os, hint + '\t');
            }
            else
                os << hint << "Return" << '\n';
        }
    };

    class StatementList : public Statement
    {
    private:
        arr<ptr<Statement>> _StatementList;

    public:
        inline static ptr<Base> Create() { return std::make_unique<StatementList>(); }
        inline void Add(ptr<Base> statement)
        {
            auto stmt = dynamic_pointer_cast<Statement>(statement);
            assert(stmt);
            _StatementList.push_back(std::move(stmt));
        }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            if (_StatementList.empty())
                os << hint << "No Statements" << '\n';
            else
                for (const auto &stmt : _StatementList)
                {
                    os << hint << "Statement: " << '\n';
                    stmt->Show(os, hint + '\t');
                }
        }
    };

} // namespace ast
