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
#include <string>
#include <iomanip>
#include <map>
#include "ErrorHandler.hpp"

namespace ast
{
    template <typename T>
    using ptr = std::unique_ptr<T>;

    template <typename T>
    using arr = std::vector<T>;

    class SymbolTable
    {
    public:
        class Symbol
        {
        public:
            llvm::Value *Value;
            bool IsRef;
            inline explicit Symbol(llvm::Value *value, bool isRef) : Value(value), IsRef(isRef) {}
        };

    private:
        SymbolTable *_Parent = nullptr;
        arr<ptr<SymbolTable>> _Children;
        std::map<std::string, llvm::Value *> _SymbolList;

    public:
        inline SymbolTable *GetParent() { return _Parent; }
        inline SymbolTable *AddChild()
        {
            _Children.push_back(std::make_unique<SymbolTable>());
            _Children.back()->_Parent = this;
            return _Children.back().get();
        }
        inline bool TryAddSymbol(const std::string &name)
        {
            return _SymbolList.find(name) == _SymbolList.end();
        }
        inline bool AddSymbol(const std::string &name, llvm::Value *sym)
        {
            // Name already exists
            if (_SymbolList.find(name) != _SymbolList.end())
                return false;
            _SymbolList.emplace(name, sym);
            return true;
        }
        inline llvm::Value *Search(const std::string &name)
        {
            auto iter = _SymbolList.find(name);
            if (iter != _SymbolList.end())
                return iter->second;
            if (!_Parent)
                return nullptr;
            return _Parent->Search(name);
        }
    };

} // namespace ast
