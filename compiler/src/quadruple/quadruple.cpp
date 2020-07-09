#include "quadruple.h"
#define MAX_SYMBOL_LENGTH 1000

Quadruple::Quadruple() {
    quadrupleList.clear();
}

void Quadruple::addQuadruple(NODE_TYPE op, string src1, string src2, string dst, bool global) {
    Quad *q = new Quad;
    q->op = op;
    q->src1 = src1;
    q->src2 = src2;
    q->dst = dst;
    q->global = global;
    quadrupleList.push_back(q);
}


//输出
void Quadruple::print(){
    for(int i=0; i<quadrupleList.size(); i++){
        cout<<'('<<quadrupleList[i]->op<<','<<quadrupleList[i]->src1<<','<<quadrupleList[i]->src2<<','<<quadrupleList[i]->dst<<')'<<endl;
    }
}


int Quadruple::length() {
    return quadrupleList.size();
}


