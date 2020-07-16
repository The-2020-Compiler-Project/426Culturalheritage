#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include<iostream>
#include<string>
#include<vector>
#include <iterator>
#include <vector>
#include <fstream>
#include <stack>
#include<deque>
#include <sstream>

using namespace std;


void OUTPUT();

void OUTPUTStoreSymbolNode();

typedef struct {
    string deli;//算符 t1=a*b
    string first;//a
    string second;//b
    string third;//t1
    int f;
    int s;
    int t;
} STY;

typedef struct {
    string data;//名字
    int type;//类型
    int state;
} SymbolNode;

//vector<STY> siyuanshi(); //返回四元式
//vector<SymbolNode> backSymbol(); //返回符号表
void TargetCode(const vector<string> &iden);


#endif // HEAD_H_INCLUDED
