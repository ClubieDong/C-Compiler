#pragma once

#include <string>
#include <iostream>

class ErrorHandler
{
public:
    static void PrintError(const std::string &msg, unsigned int row,
                           unsigned int colStart, unsigned int colEnd)
    {
        std::cerr << row << ':' << colStart << '-' << colEnd << '\t';
        std::cerr << "Error: " << msg << '\n';
    }

    static void PrintWarning(const std::string &msg, unsigned int row,
                           unsigned int colStart, unsigned int colEnd)
    {
        std::cerr << row << ':' << colStart << '-' << colEnd << '\t';
        std::cerr << "Warning: " << msg << '\n';
    }
};