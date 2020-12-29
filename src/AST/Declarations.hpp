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

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
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

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "CustomType: " << _ID << '\n';
        }
    };

    class VarDecl : public Base
    {
    private:
        std::string _Name;

    public:
        inline explicit VarDecl(const std::string &name) : _Name(name) {}

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "VarDecl: " << _Name << '\n';
        }
    };

    class ArrayDecl : public VarDecl
    {
    private:
        ptr<VarDecl> _Type;
        std::optional<size_t> _Size;

    public:
        inline explicit ArrayDecl(const std::string &name, ptr<Base> &type,
                                  std::optional<size_t> size = std::nullopt)
            : VarDecl(name), _Type(to<VarDecl>(type)), _Size(size) {}

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "ArrayDecl: ";
            if (_Size)
                os << "(Size = " << *_Size << ')';
            os << '\n';
            _Type->Show(os, hint + '\t');
        }
    };

    class PointerDecl : public VarDecl
    {
    private:
        ptr<VarDecl> _Type;

    public:
        inline explicit PointerDecl(const std::string &name, ptr<Base> &type)
            : VarDecl(name), _Type(to<VarDecl>(type)) {}

        inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
        {
            os << hint << "Pointer: \n";
            _Type->Show(os, hint + '\t');
        }
    };

    // class DeclStmt : public Statement
    // {
    // private:
    //     ptr<TypePrimitive> _Primitive;
    //     arr<ptr<TypeDecoration>> _DecorationList;

    // public:
    //     inline explicit DeclStmt(ptr<TypePrimitive> primitive, ptr<TypeDecoration> decoration)
    //         : _Primitive(std::move(primitive)) { _DecorationList.push_back(std::move(decoration)); }
    //     inline static ptr<Base> Create(ptr<Base> elementType)
    //     {
    //         auto type = dynamic_pointer_cast<TypeDecoration>(elementType);
    //         assert(type);
    //         return std::make_unique<PointerType>(type);
    //     }

    //     inline virtual void Show(std::ostream &os, const std::string &hint = "") const override
    //     {
    //         os << hint << "Pointer: ";
    //         _ElementType->Show(os, hint + '\t');
    //     }
    // };

} // namespace ast