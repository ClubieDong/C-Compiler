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

    class TypePrimitive;
    class Decl;

    class SymbolTable
    {
    public:
        struct Symbol
        {
            TypePrimitive *Type;
            Decl *TypeDeco;
            bool Assignable;

            inline explicit Symbol() = default;
            inline explicit Symbol(TypePrimitive *type, Decl *deco, bool assignable)
                : Type(type), TypeDeco(deco), Assignable(assignable) {}
        };

    private:
        SymbolTable *_Parent = nullptr;
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
        inline bool AddSymbol(const std::string &name, TypePrimitive *type, Decl *deco)
        {
            // Name already exists
            if (_SymbolList.find(name) != _SymbolList.end())
                return false;
            _SymbolList.emplace(name, Symbol(type, deco, true));
            return true;
        }
        inline const Symbol* Search(const std::string& name)
        {
            auto iter = _SymbolList.find(name);
            if (iter != _SymbolList.end())
                return &iter->second;
            if (!_Parent)
                return nullptr;
            return _Parent->Search(name);
        }
    };

} // namespace ast