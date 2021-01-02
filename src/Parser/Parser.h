// Generated by Bisonc++ V6.03.00 on Sat, 31 Oct 2020 09:16:14 +0800

#pragma once

// $insert baseclass
#include "Parserbase.h"
// $insert scanner.h
#include "../Scanner/Scanner.h"

#undef Parser
// CAVEAT: between the baseclass-include directive and the
// #undef directive in the previous line references to Parser
// are read as ParserBase.
// If you need to include additional headers in this file
// you should do so after these comment-lines.

#include <memory>
#include <iomanip>
#include <set>
#include <vector>
#include <sstream>
#include "../ErrorHandler.hpp"

class Parser : public ParserBase
{
private:
    Scanner *_Scanner;
    std::unique_ptr<ast::Base> _Root;
    inline static constexpr const char *TOKEN_NAMES[] =
        {"void", "int", "return", "if", "else", "while", "struct",
         "num", "ID", "PTR", "ARR", "'<='", "'>='", "'=='", "'!='"};
    inline static constexpr unsigned int TOKEN_COUNT = sizeof(TOKEN_NAMES) / sizeof(*TOKEN_NAMES);

public:
    inline Parser(Scanner &scanner) : _Scanner(&scanner) {}
    int parse();

    inline std::unique_ptr<ast::Base> GetRoot() { return std::move(_Root); }

private:
    std::vector<int> ValidTokens();

    inline std::string Tok2Str(int token)
    {
        if (token > 256)
            return TOKEN_NAMES[token - 257];
        return std::string("\'") + static_cast<char>(token) + "\'";
    }
    inline std::string MachedStr()
    {
        if (_Scanner->matched() == "")
            return "end of file";
        return '"' + _Scanner->matched() + '"';
    }
    // called on (syntax) errors
    inline void error()
    {
        auto validTokens = ValidTokens();
        std::ostringstream ss;
        if (validTokens.empty())
            ss << "Unexpected " << MachedStr();
        else if (validTokens.size() == 1)
            ss << "Expect " << Tok2Str(validTokens.front()) << " before " << MachedStr();
        else
        {
            ss << "Expect " << Tok2Str(validTokens.front());
            for (size_t i = 1; i < validTokens.size() - 1; ++i)
                ss << ", " << Tok2Str(validTokens[i]);
            ss << " or " << Tok2Str(validTokens.back()) << " before " << MachedStr();
        }
        ErrorHandler::PrintError(ss.str(), _Scanner->GetLocation());
    }

    // returns the next token from the
    // lexical scanner.
    inline int lex() { return _Scanner->lex(); }
    // use, e.g., d_token, d_loc
    inline void print() const {}
    // Suppress all exceptions
    void exceptionHandler(std::exception const &exc) {}

    // support functions for parse():
    void executeAction_(int ruleNr);
    void errorRecovery_();
    void nextCycle_();
    void nextToken_();
    void print_();
};
