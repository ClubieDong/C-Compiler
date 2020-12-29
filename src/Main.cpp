#include "Scanner/Scanner.ih"
#include "Parser/Parser.ih"

int main()
{
    Scanner s;
    Parser p(s);
    p.parse();
    return 0;
}