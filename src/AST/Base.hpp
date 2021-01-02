#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <optional>
#include <string>

#define ast_assert(x) if (!x) throw std::runtime_error("")

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

        inline virtual void Show(std::ostream &os = std::cout, const std::string &hint = "") const {};
    };

    class ID : public Base
    {
    private:
        std::string _ID;

    public:
        inline explicit ID(const std::string &id) : _ID(id) {}

        inline virtual void Show(std::ostream &os, const std::string &hint) const override
        {
            os << hint << "ID: " << _ID << '\n';
        }
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

    inline void PrintError(const std::string &msg)
    {
    }
} // namespace ast
