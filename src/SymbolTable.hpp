#pragma once

#include <memory>
#include <vector>
#include <string>

namespace sym
{
    template <typename T>
    using ptr = std::unique_ptr<T>;

    template <typename T>
    using arr = std::vector<T>;

    class SymbolTable
    {
    public:
        struct Symbol
        {
            // std::string Name;
            // ptr<type::Type> Type;
        };

    private:
        SymbolTable *_Parent;
        arr<ptr<SymbolTable>> _Children;
        arr<Symbol> _SymbolList;

    public:
        inline SymbolTable *GetParent() { return _Parent; }
        inline SymbolTable *AddChild()
        {
            _Children.push_back(std::make_unique<SymbolTable>());
            _Children.back()->_Parent = this;
            return _Children.back().get();
        }
    };

} // namespace sym