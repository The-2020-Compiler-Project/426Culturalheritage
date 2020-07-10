#include <fstream>
#include <iostream>
#include <algorithm>
#include "lex.h"
#include "parse.h"
#include "Grammar.h"
#include "semantic.h"
#include "gen.h"

extern vector<STY> FourStyle;
extern vector<SymbolNode> Symbol;

int main() {
    const char *path = R"(D:\source\426Culturalheritage\compiler\tests\long_program.txt)";
    Init_lexer(path);

    auto AST = syntax();
    //auto program = get_AST();

    auto q = treeToQuad(AST);


    displayAST(AST);

    q->print(std::cout);
    std::cout << "*********" << std::endl;

    auto it = getIdenTable();

    it->print(std::cout);


    transform(q->quadrupleList.begin(), q->quadrupleList.end(),
              back_inserter(FourStyle),
              [](Quad *l) { return STY{l->op, l->src1, l->src2, l->dst, l->f, l->s, l->t}; });
    FourStyle.push_back({"func_use", "_", "_", "main"});

    TargetCode();

    return 0;

}
