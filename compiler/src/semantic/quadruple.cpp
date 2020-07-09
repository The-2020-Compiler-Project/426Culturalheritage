#include "semantic.h"

#define MAX_SYMBOL_LENGTH 1000
using namespace std;

Quadruple::Quadruple() {
    quadrupleList.clear();
}

void Quadruple::addQuadruple(string op, string src1, string src2, string dst, int f, int s, int t) {
    Quad *q = new Quad;
    q->op = op;
    q->src1 = src1;
    q->src2 = src2;
    q->dst = dst;
    quadrupleList.push_back(q);
}


//输出
void Quadruple::print(ostream &out) {
    for (auto &i : quadrupleList) {
        out << "( " << i->op << " , "
            << i->src1 << " , "
            << i->src2 << " , "
            << i->dst << " ) " << endl;
    }
}


int Quadruple::length() const {
    return quadrupleList.size();
}


