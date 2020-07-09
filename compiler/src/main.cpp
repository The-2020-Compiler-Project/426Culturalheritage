#include <fstream>
#include <iostream>
#include "lex.h"
#include "parse.h"
#include "Grammar.h"
#include "semantic.h"

int main() {
    //const char *path = R"(D:\source\426Culturalheritage\compiler\tests\long_program.txt)";
    //Init_lexer(path);

    //auto AST = syntax();
    auto program = get_AST();

    auto q = treeToQuad( program );

    q->print();


    displayAST(AST);
    auto q = treeToQuad(AST);


    std::ofstream four("FourStream", std::ios::out);
    q->print(four);
    std::cout << "*********" << std::endl;

    auto it = getIdenTable();


    it->print(std::cout);

    return 0;

}
