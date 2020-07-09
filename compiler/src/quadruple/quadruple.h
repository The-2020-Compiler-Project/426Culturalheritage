#ifndef GRAMMATREE_QUADRUPLE_H
#define GRAMMATREE_QUADRUPLE_H

#include <iostream>
#include <string>
#include <vector>
#include "lex.h"
#include "parse.h"
using namespace std;

struct Quad{
    NODE_TYPE op;
    string src1;
    string src2;
    string dst;
    bool global;
};

class Quadruple{
public:
    Quadruple();
    vector<Quad *> quadrupleList;

    void addQuadruple(NODE_TYPE op, string src1, string src2, string dst, bool global = false);
    void print();
    int length();

private:
};

#endif //COMPILER_QUADRUPLE_H
