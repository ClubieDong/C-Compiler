#pragma once

#include <memory>
#include <vector>
#include <string>
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
            inline explicit Symbol(llvm::Value* value, bool isRef) : Value(value), IsRef(isRef) {}
        };

        private : SymbolTable *_Parent = nullptr;
        arr<ptr<SymbolTable>> _Children;
        std::map<std::string, Symbol> _SymbolList;

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
        inline bool AddSymbol(const std::string &name, Symbol sym)
        {
            // Name already exists
            if (_SymbolList.find(name) != _SymbolList.end())
                return false;
            _SymbolList.emplace(name, sym);
            return true;
        }
        inline std::optional<Symbol> Search(const std::string &name)
        {
            auto iter = _SymbolList.find(name);
            if (iter != _SymbolList.end())
                return iter->second;
            if (!_Parent)
                return std::nullopt;
            return _Parent->Search(name);
        }
    };

} // namespace ast