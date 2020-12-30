#pragma once

#include "Base.hpp"

namespace ast
{
    class Expression : public Base
    {
    };

    class Constant : public Expression
    {
    private:
        int _Value;

    public:
        inline explicit Constant(int value) : _Value(value) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
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

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
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
        inline explicit BiOpExpr(ptr<Base> &left, ptr<Base> &right, BiOp op)
            : _Left(to<Expression>(left)), _Right(to<Expression>(right)), _Op(op) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"ASSIGN", "LESS", "GREATER", "LESS_EQUAL", "GREATER_EQUAL",
                                                  "EQUAL", "NOT_EQUAL", "ADD", "SUB", "MUL", "DIV"};
            os << hint << "BiOpExpr: " << ENUM_NAMES[_Op] << '\n';
            os << hint << "\tLeft: \n";
            _Left->Show(os, hint + "\t\t");
            os << hint << "\tRight: \n";
            _Right->Show(os, hint + "\t\t");
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
        inline explicit UnOpExpr(ptr<Base> &operand, UnOp op)
            : _Operand(to<Expression>(operand)), _Op(op) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"NEG"};
            os << hint << "UnOpExpr: " << ENUM_NAMES[_Op] << '\n';
            _Operand->Show(os, hint + '\t');
        }
    };

    class CallExpr : public Expression
    {
    private:
        ptr<Expression> _Func;
        arr<ptr<Expression>> _ArgList;

    public:
        inline explicit CallExpr() = default;
        inline explicit CallExpr(ptr<Base> &arg) { _ArgList.push_back(to<Expression>(arg)); }

        inline void AddArg(ptr<Base> &arg) { _ArgList.push_back(to<Expression>(arg)); }
        inline void SetFunc(ptr<Base> &func) { _Func = to<Expression>(func); }

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "CallExpr: \n";
            os << hint << "\tFunction: \n";
            _Func->Show(os, hint + "\t\t");
            if (_ArgList.empty())
                os << hint << "\tNo Arguments\n";
            else
                for (const auto &arg : _ArgList)
                {
                    os << hint << "\tArgument: \n";
                    arg->Show(os, hint + "\t\t");
                }
        }
    };

} // namespace ast