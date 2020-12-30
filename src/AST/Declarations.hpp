#pragma once

#include "Base.hpp"

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
            INT
        };

    private:
        Type _Type;

    public:
        inline explicit BasicType(Type type) : _Type(type) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            constexpr const char *ENUM_NAMES[] = {"VOID", "INT"};
            os << hint << "BasicType: " << ENUM_NAMES[_Type] << '\n';
        }
    };

    class CustomType : public TypePrimitive
    {
    private:
        std::string _ID;

    public:
        inline explicit CustomType(const std::string &id) : _ID(id) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "CustomType: " << _ID << '\n';
        }
    };

    class VarDecl : public Base
    {
    };

    class PlainVarDecl : public VarDecl
    {
    private:
        std::string _Name;

    public:
        inline explicit PlainVarDecl(const std::string &name) : _Name(name) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "PlainVarDecl: " << _Name << '\n';
        }
    };

    class ArrayVarDecl : public VarDecl
    {
    private:
        ptr<VarDecl> _Type;
        ptr<Expression> _Size;

    public:
        inline explicit ArrayVarDecl(ptr<Base> &type)
            : _Type(to<VarDecl>(type)) {}
        inline explicit ArrayVarDecl(ptr<Base> &type, ptr<Base> &size)
            : _Type(to<VarDecl>(type)), _Size(to<Expression>(size)) {}

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
    };

    class PointerVarDecl : public VarDecl
    {
    private:
        ptr<VarDecl> _Type;

    public:
        inline explicit PointerVarDecl(ptr<Base> &type) : _Type(to<VarDecl>(type)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "Pointer: \n";
            _Type->Show(os, hint + '\t');
        }
    };

    class InitVarDecl : public Base
    {
    private:
        ptr<VarDecl> _Var;
        ptr<Expression> _Init;

    public:
        inline explicit InitVarDecl(ptr<Base> &var)
            : _Var(to<VarDecl>(var)) {}
        inline explicit InitVarDecl(ptr<Base> &var, ptr<Base> &init)
            : _Var(to<VarDecl>(var)), _Init(to<Expression>(init)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "InitVarDecl: \n";
            os << hint << "\tVar: \n";
            _Var->Show(os, hint + "\t\t");
            if (_Init)
            {
                os << hint << "\tInit: \n";
                _Init->Show(os, hint + "\t\t");
            }
        }
    };

    class VarDeclStmt : public Statement
    {
    private:
        ptr<TypePrimitive> _Type;
        arr<ptr<InitVarDecl>> _VarList;

    public:
        inline explicit VarDeclStmt(ptr<Base> &var) { _VarList.push_back(to<InitVarDecl>(var)); }

        inline void AddVar(ptr<Base> &var) { _VarList.push_back(to<InitVarDecl>(var)); }
        inline void SetType(ptr<Base> &type) { _Type = to<TypePrimitive>(type); }

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "VarDeclStmt: \n";
            os << hint << "\tType: \n";
            _Type->Show(os, hint + "\t\t");
            for (const auto &var : _VarList)
            {
                os << hint << "\tVar: \n";
                var->Show(os, hint + "\t\t");
            }
        }
    };

} // namespace ast