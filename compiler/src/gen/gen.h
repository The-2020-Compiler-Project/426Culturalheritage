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

struct block_begin_end {
    int begin;
    int end;
};

struct RDL {
    string data;  //�Ĵ����е�����
    int state;   //�����ݵĻ�Ծ��Ϣ��y-1;n-0;
};
typedef struct {
    string deli;//��� t1=a*b
    string first;//a
    string second;//b
    string third;//t1
    int f;
    int s;
    int t;
} STY;

typedef struct {
    string data;//����
    int type;//����
    int state;
} SymbolNode;

//vector<STY> siyuanshi(); //������Ԫʽ
//vector<SymbolNode> backSymbol(); //���ط��ű�
void TargetCode(const vector<string> &iden, const string &outputpath);


#endif // HEAD_H_INCLUDED
