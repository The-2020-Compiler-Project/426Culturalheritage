#include "quadruple.h"
#define MAX_SYMBOL_LENGTH 1000

Quadruple::Quadruple() {
    quadrupleList.clear();
}

void Quadruple::addQuadruple(string op, string src1, string src2, string dst, int f,int s,int t) {
    Quad *q = new Quad;
    q->op = op;
    q->src1 = src1;
    q->src2 = src2;
    q->dst = dst;
    quadrupleList.push_back(q);
}


//输出
void Quadruple::print(){
    for(int i=0; i<quadrupleList.size(); i++){
        cout<<'('<<quadrupleList[i]->op<<','
        <<quadrupleList[i]->src1<<" ("<<quadrupleList[i]->f<<") ,"
        <<quadrupleList[i]->src2<<" ("<<quadrupleList[i]->s<<") ,"
        <<quadrupleList[i]->dst <<" ("<<quadrupleList[i]->t<<") "<<endl;
    }
}


int Quadruple::length() const {
    return quadrupleList.size();
}


