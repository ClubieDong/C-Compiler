#pragma once

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

#include <memory>
#include <vector>
#include <iostream>
#include <optional>
#include <string>
#include "../SymbolTable.hpp"
#include "../ErrorHandler.hpp"

#define ast_assert(x) \
    if (!x)           \
    throw std::runtime_error("")

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

        inline virtual void Show(std::ostream &os = std::cout, const std::string &hint = "") const {}
        inline virtual bool Analyze(SymbolTable *syms) { return true; }
    };

    class ID : public Base
    {
    private:
        std::string _ID;
        ErrorHandler::Location _Location;

    public:
        inline explicit ID(const std::string &id, const ErrorHandler::Location& loc)
            : _ID(id), _Location(loc) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "ID: " << _ID << '\n';
        }

        inline std::string GetName() const { return _ID; }
        inline const ErrorHandler::Location& GetLocation() const { return _Location; }
    };

    template <typename Derived>
    inline ptr<Derived> to(ptr<Base> &base)
    {
        if (!base)
            return ptr<Derived>(nullptr);
        auto p = dynamic_cast<Derived *>(base.get());
        ast_assert(p);
        ptr<Derived> res(p);
        base.release();
        return res;
    }

    template <typename Derived>
    inline Derived *cast(ptr<Base> &base)
    {
        ast_assert(base);
        auto p = dynamic_cast<Derived *>(base.get());
        ast_assert(p);
        return p;
    }
} // namespace ast
