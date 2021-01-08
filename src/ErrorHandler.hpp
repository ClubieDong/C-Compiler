#pragma once

#include <string>
#include <iostream>

class ErrorHandler
{
public:
    struct Location
    {
        unsigned int Row = 1, ColStart = 1, ColEnd = 1;
        inline explicit Location() = default;
        inline explicit Location(unsigned int row, unsigned int colStart, unsigned int colEnd)
            : Row(row), ColStart(colStart), ColEnd(colEnd) {}
    };

    inline static void PrintError(const std::string &msg, const Location &loc, bool isWarning = false)
    {
        std::cerr << loc.Row << ':' << loc.ColStart << '-' << loc.ColEnd << '\t';
        std::cerr << (isWarning ? "Warning" : "Error") << ": " << msg << '\n';
    }

    inline static void PrintWarning(const std::string &msg, const Location &loc) { PrintError(msg, loc, true); }
};
