#pragma once

#include <sstream>
#include "Base.hpp"
#include "Expressions.hpp"
#include "Statements.hpp"

namespace ast
{
    class TypePrimitive : public Base
    {
    public:
        inline virtual llvm::Type *TypeGen(llvm::LLVMContext &context) = 0;
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

        inline virtual llvm::Type *TypeGen(llvm::LLVMContext &context) override
        {
            switch (_Type)
            {
            case VOID:
                return llvm::Type::getVoidTy(context);
            case INT:
                return llvm::Type::getInt32Ty(context);
            case BOOL:
                return llvm::Type::getInt1Ty(context);
            default:
                return nullptr;
            }
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

        inline virtual llvm::Type *TypeGen(llvm::LLVMContext &context) override
        {
            // TODO
            return nullptr;
        }
    };
}
