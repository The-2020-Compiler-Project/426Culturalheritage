#include <iostream>
#include "lex.h"
#include "parse.h"
#include "Grammar.h"

int main() {
    const char *path = R"(D:\source\426Culturalheritage\compiler\tests\long_program.txt)";
    Init_lexer(path);

    /*
    auto AST = syntax();
     */

    return 0;
}
