#ifndef GRAMMATREE_QUADRUPLE_H
#define GRAMMATREE_QUADRUPLE_H

#include <iostream>
#include <string>
#include <vector>
#include "lex.h"
#include "parse.h"
using namespace std;

struct Quad{
    string op;
    string src1;
    string src2;
    string dst;
    int f;
    int s;
    int t;
};

class Quadruple{
public:
    Quadruple();
    vector<Quad *> quadrupleList;

    void addQuadruple(string op, string src1, string src2, string dst, int f=0, int s=0, int t=0);
    void print();
    int length() const;

private:
};

#endif //COMPILER_QUADRUPLE_H
