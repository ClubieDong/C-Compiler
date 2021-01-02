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

            inline explicit Symbol(TypePrimitive *type, Decl *deco)
                : Type(type), TypeDeco(deco) {}
        };

    private:
        SymbolTable *_Parent;
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
            _SymbolList.emplace(name, Symbol(type, deco));
            return true;
        }
    };

} // namespace ast