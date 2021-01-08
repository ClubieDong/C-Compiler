#include "AST.hpp"

namespace ast
{
    bool FuncDecl::SetParamName(SymbolTable &syms, llvm::Function *func, llvm::IRBuilder<> &builder)
    {
        bool success = true;
        unsigned int idx = 0;
        for (auto &p : func->args())
        {
            const auto &name = _ParamList[idx]._Decl->GetName();
            p.setName(name);
            llvm::Value *value;
            if (dynamic_cast<ReferenceDecl *>(_ParamList[idx]._Decl.get()))
                value = &p;
            else
            {
                value = builder.CreateAlloca(p.getType(), 0, "");
                builder.CreateStore(&p, value);
            }
            if (!syms.AddSymbol(name, value))
            {
                std::ostringstream ss;
                ss << "Redeclaration of '" << name << '\'';
                ErrorHandler::PrintError(ss.str(), _ParamList[idx]._Decl->GetLocation());
                success = false;
            }
            ++idx;
        }
        return success;
    }
} // namespace ast
