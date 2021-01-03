#include "AST.hpp"

namespace ast
{
    // bool Constant::Analyze(SymbolTable *syms)
    // {
    //     _Type = std::make_unique<BasicType>(BasicType::INT);
    //     _TypeDeco = std::make_unique<VarDecl>();
    //     _ExprType = SymbolTable::Symbol(_Type.get(), _TypeDeco.get(), false);
    //     return true;
    // }

    llvm::Type *ArrayDecl::TypeGen(llvm::LLVMContext &context, llvm::Type *type)
    {
        if (dynamic_cast<ReferenceDecl *>(_Type.get()))
        {
            ErrorHandler::PrintError("Array of reference is not allowed", _Type->GetLocation());
            return nullptr;
        }
        type = _Type->TypeGen(context, type);
        if (!type)
            return nullptr;

        // TODO
        // return llvm::ArrayType::get(_Type->TypeGen(context, type), /* _Size */);
        return nullptr;
    }

    llvm::Type *PointerDecl::TypeGen(llvm::LLVMContext &context, llvm::Type *type)
    {
        if (dynamic_cast<ReferenceDecl *>(_Type.get()))
        {
            ErrorHandler::PrintError("Pointer to reference is not allowed", _Type->GetLocation());
            return nullptr;
        }
        type = _Type->TypeGen(context, type);
        if (!type)
            return nullptr;
        return llvm::PointerType::get(type, 0);
    }
} // namespace ast