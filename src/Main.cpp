#include <iostream>
#include <iomanip>
#include "Scanner/Scanner.ih"
#include "Parser/Parser.ih"
#include "SymbolTable.hpp"

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
        std::cout << std::setw(10) << s.GetLocation().Row;
        std::cout << std::setw(10) << s.GetLocation().ColStart;
        std::cout << std::setw(10) << s.GetLocation().ColEnd;
        std::cout << '\n';
    };

    while (true)
    {
        auto tok = s.lex();
        if (tok == 0)
            break;
        if (tok < 256)
            output("CHAR");
        output(Parser::TOKEN_NAMES[tok - 257]);
    }
}

void TestParser()
{
    Scanner s;
    Parser p(s);
    if (p.parse())
        return;
    auto astRoot = p.GetRoot();
    astRoot->Show();
}

void TestLLVM()
{
    Scanner s;
    Parser p(s);
    if (p.parse())
        return;
    auto astRoot = p.GetRoot();
    llvm::LLVMContext context;
    llvm::Module mod("ClubieModule", context);
    auto success = ast::cast<ast::DeclarationList>(astRoot)->CodeGen(context, mod);
    if (success)
        mod.print(llvm::outs(), nullptr);
}

int main()
{
    TestLLVM();
    return 0;
}