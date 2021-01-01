// Generated by Bisonc++ V6.03.00 on Sat, 31 Oct 2020 09:16:14 +0800

#ifndef Parser_h_included
#define Parser_h_included

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

class Parser : public ParserBase
{
private:
    Scanner *_Scanner;
    std::unique_ptr<ast::Base> _Root;

public:
    Parser(Scanner &scanner) : _Scanner(&scanner) {}
    int parse();

    std::unique_ptr<ast::Base> GetRoot() { return std::move(_Root); }

private:
    void error(); // called on (syntax) errors
    int lex();    // returns the next token from the
                  // lexical scanner.
    void print(); // use, e.g., d_token, d_loc
    void exceptionHandler(std::exception const &exc);

    // support functions for parse():
    void executeAction_(int ruleNr);
    void errorRecovery_();
    void nextCycle_();
    void nextToken_();
    void print_();
};

#endif
