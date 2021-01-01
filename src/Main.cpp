#include <iostream>
#include <iomanip>
#include "Scanner/Scanner.ih"
#include "Parser/Parser.ih"

void TestScanner()
{
    Scanner s;

    std::cout << std::setw(10) << "Token";
    std::cout << std::setw(15) << "Matched";
    std::cout << std::setw(10) << "Row";
    std::cout << std::setw(10) << "ColStart";
    std::cout << std::setw(10) << "ColEnd";
    std::cout << "\n\n";
    auto output = [&](const auto &n) {
        std::cout << std::setw(10) << n;
        std::cout << std::setw(15) << s.matched();
        std::cout << std::setw(10) << s.GetRow();
        std::cout << std::setw(10) << s.GetColumnStart();
        std::cout << std::setw(10) << s.GetColumnEnd();
        std::cout << '\n';
    };

    while (true)
    {
        auto tok = s.lex();
        if (tok == 0)
            break;
        switch (tok)
        {
        case Parser::VOID:
            output("VOID");
            break;
        case Parser::INT:
            output("INT");
            break;
        case Parser::RETURN:
            output("RETURN");
            break;
        case Parser::IF:
            output("IF");
            break;
        case Parser::ELSE:
            output("ELSE");
            break;
        case Parser::WHILE:
            output("WHILE");
            break;
        case Parser::STRUCT:
            output("STRUCT");
            break;
        case Parser::NUM:
            output("NUM");
            break;
        case Parser::ID_TEXT:
            output("ID");
            break;
        case Parser::PTR:
            output("PTR");
            break;
        case Parser::ARR:
            output("ARR");
            break;
        case Parser::LE:
            output("LE");
            break;
        case Parser::GE:
            output("GE");
            break;
        case Parser::EQ:
            output("EQ");
            break;
        case Parser::NE:
            output("NE");
            break;
        default:
            output("CHAR");
            break;
        }
    }
}

void TestParser()
{
    Scanner s;
    Parser p(s);
    p.parse();
    auto astRoot = p.GetRoot();
    astRoot->Show();
}

int main()
{
    TestScanner();
    return 0;
}