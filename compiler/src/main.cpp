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

int main(int argc, char **argv) {
    string inputpath, outputpath;
    if (argc == 1) {
        cout << "Path of input file: ";
        getline(cin, inputpath);

        cout << "Path of output file: ";
        getline(cin, outputpath);
    } else if (argc == 3) {
        inputpath = argv[1];
        outputpath = argv[2];
    } else {
        cerr << "Error: need 2 parameters\n";
        cout << "Usage: compile [path of input] [path of output]";
        exit(0);
    }
    //const char *path = argv[1];R"(D:\source\426Culturalheritage\compiler\tests\long_program.txt)";
    Init_lexer(inputpath.c_str());

    auto AST = syntax();
    //auto program = get_AST();

    cout << "Output some of the AST:\n";
    displayAST(AST);
    auto q = treeToQuad(AST);


    q->print(std::cout);
    std::cout << "*********" << std::endl;

    //将中间代码生成中的四元式给目标代码生成
    transform(q->quadrupleList.begin(), q->quadrupleList.end(),
              back_inserter(FourStyle),
              [](Quad *l) { return STY{l->op, l->src1, l->src2, l->dst, l->f, l->s, l->t}; });
    FourStyle.push_back({"func_use", "_", "_", "main"});

    //将中间代码生成中的符号表给目标代码生成
    auto it = getIdenTable();

    it->print(std::cout);

    transform(it->idTable.begin(), it->idTable.end(), back_inserter(Symbol),
              [](pair<string, pair<string, bool>> a) {
                  return SymbolNode{a.first, (a.second.second ? 0 : 1), (a.second.second ? 0 : 1)};
              });
    vector<string> iden;
    for (auto &&[x, y]:it->idTable) {
        iden.push_back(x);
    }

    TargetCode(iden, outputpath);
    cout << "ASM code output to " << outputpath;

    return 0;

}
