/**
 * AlbertTan
 * grammar Tree To Quadruple
 * 当前进度：
 * 当前问题：
 */

#ifndef COMPILER_SEMANTIC_H
#define COMPILER_SEMANTIC_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "parse.h"
#include "lex.h"
#include "error_diagnostics.h"

class idenTable {
public:
    std::map<std::string, std::pair<std::string, bool>> idTable;

    bool exist(const char *s);

    std::string getValue(const char *s);

    template<bool isTemp>
    void append(const std::string &s, const std::string &value = "");

    idenTable() {};

    ~idenTable() = default;

    void print(std::ostream &out);
};

class constTable {
public:
    std::vector<double> conTable;

    int getIndex(double num);

    void append(double num);

    constTable() {};

    ~constTable() = default;
};

struct Quad {
    std::string op;
    std::string src1;
    std::string src2;
    std::string dst;
    int f;
    int s;
    int t;
};

class Quadruple {
public:
    Quadruple();

    std::vector<Quad *> quadrupleList;

    void
    addQuadruple(std::string op, std::string src1, std::string src2, std::string dst, int f = -1, int s = -1,
                 int t = -1);

    int length() const;

    void print(std::ostream &out);

private:
};


/*
// 返回变量
class templeNum{
public:
    static int count;
    char *name;
    double value;
    int index;
    templeNum(char* name,double value,int index):
        name(name),value(value),index(index){ count++; }
};
int templeNum::count = 0;
 */
// 语法树生成四元式
Quadruple *treeToQuad(Nodebase *p);

// 字符串(name)转double
double strToDouble(const char *s);

// double转char*
std::string doubleToStr(double n);

// 函数定义
void FuncDeclare(Quadruple *quadruple, Declaration_node *p);

// 获取数值
double getNumber(Quadruple *quadruple, Expression_Statement_node *p);

// 遍历语法树
void traverse(Quadruple *quadruple, Nodebase *p);

// 赋值语句
void Assignment(Quadruple *quadruple, Expression_Statement_node *p);

// 获取赋值语句右孩子数值
double getNumber(Expression_Statement_node *p);

// IF语句
void IF(Quadruple *quadruple, Expression_Statement_node *p);

// 函数定义
void funDec(Quadruple *quadruple, Expression_Statement_node *p);

// for While语句
void FOR(Quadruple *quadruple, Expression_Statement_node *p);

// Return语句
void RT(Quadruple *quadruple, Expression_Statement_node *p);

idenTable *getIdenTable();

std::string calculate(Quadruple *quadruple, Nodebase *p);

bool hasSon(Nodebase *p);

#endif //COMPILER_SEMANTIC_H
