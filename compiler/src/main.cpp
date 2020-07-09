#include <iostream>
#include "lex.h"
#include "parse.h"
#include "Grammar.h"
#include "quadruple.h"
#include "semantic.h"
#include "tables.h"

int main() {
    //const char *path = R"(D:\source\426Culturalheritage\compiler\tests\long_program.txt)";
    //Init_lexer(path);

    //auto AST = syntax();
    auto program = get_AST();

    auto q = treeToQuad( program );

    q->print();

    return 0;
}
