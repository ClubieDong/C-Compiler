#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "Scanner/Scanner.ih"
#include "Parser/Parser.ih"
#include "SymbolTable.hpp"

void TestScanner(std::ifstream& input, std::ofstream& lexOutput)
{
    Scanner s(input, std::cerr);

    lexOutput << std::setw(10) << "Token";
    lexOutput << std::setw(15) << "Matched";
    lexOutput << std::setw(10) << "Row";
    lexOutput << std::setw(10) << "ColStart";
    lexOutput << std::setw(10) << "ColEnd";
    lexOutput << "\n\n";
    auto output = [&](const auto &n) {
        lexOutput << std::setw(10) << n;
        lexOutput << std::setw(15) << s.matched();
        lexOutput << std::setw(10) << s.GetLocation().Row;
        lexOutput << std::setw(10) << s.GetLocation().ColStart;
        lexOutput << std::setw(10) << s.GetLocation().ColEnd;
        lexOutput << '\n';
    };

    while (true)
    {
        auto tok = s.lex();
        if (tok == 0)
            break;
        if (tok < 256)
            output("CHAR");
        else
            output(Parser::TOKEN_NAMES[tok - 257]);
    }
}

void TestLLVM(std::ifstream& input, std::ofstream& astOutput, llvm::raw_fd_ostream& irOutput)
{
    Scanner s(input, std::cerr);
    Parser p(s);
    if (p.parse())
        return;
    auto astRoot = p.GetRoot();
    astRoot->Show(astOutput);
    llvm::LLVMContext context;
    llvm::Module mod("Module", context);
    auto success = ast::cast<ast::DeclarationList>(astRoot)->CodeGen(context, mod);
    if (success)
        mod.print(irOutput, nullptr);
}

void ShowHelp(const char *name)
{
    std::cerr << "Usage: " << name << " <file>\n";
    std::cerr << "  Compile the specified file\n";
    std::cerr << "Output: \n";
    std::cerr << "  <file>.lex  contains the result of scanner\n";
    std::cerr << "  <file>.ast  contains the Abstract Syntax Tree(AST)\n";
    std::cerr << "  <file>.ir   contains the generated LLVM IR code\n";
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        ShowHelp(argv[0]);
        return 0;
    }
    std::string file(argv[1]);
    if (file == "-h" || file == "--help")
    {
        ShowHelp(argv[0]);
        return 0;
    }

    std::ifstream input1(file);
    std::ofstream lexOutput(file + ".lex");
    TestScanner(input1, lexOutput);

    std::error_code ec;
    std::ifstream input2(file);
    std::ofstream astOutput(file + ".ast");
    llvm::raw_fd_ostream irOutput(file + ".ir", ec);
    TestLLVM(input2, astOutput, irOutput);

    return 0;
}
