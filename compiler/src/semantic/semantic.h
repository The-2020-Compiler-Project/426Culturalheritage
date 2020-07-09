/**
 * AlbertTan
 * grammar Tree To Quadruple
 * 当前进度：程序入口四元式生成、number、char全局变量的声明与定义的四元式生成
 * 当前问题：
 */

#ifndef COMPILER_SEMANTIC_H
#define COMPILER_SEMANTIC_H

#include <iostream>
#include "../parse/parse.h"
#include "../lexical/lex.h"
#include "../quadruple/quadruple.h"
#include "../tables/tables.h"

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
// 语法树生成四元式
Quadruple *treeToQuad( constTable *CT,idenTable *IT );

// 字符串(name)转double
double strToDouble( const char *s );

// double转char*
char* doubleToStr( double n );

// 函数定义
void FuncDeclare( Quadruple *quadruple, Declaration_node *p, constTable *CT, idenTable *IT );

// 获取数值
double getNumber( Quadruple *quadruple, Expression_Statement_Node *p,idenTable *IT );

// 遍历语法树
void traverse( Quadruple *quadruple, Nodebase *p, constTable *CT, idenTable *IT );

// 赋值语句
void Assignment( Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT );

// 获取赋值语句右孩子数值
double getNumber( Expression_Statement_Node *p );

// IF语句
void IF(  Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT);

// 函数定义
void funDec( Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT);

// While语句
void WH(Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT);

// Return语句
void RT( Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT);
#endif //COMPILER_SEMANTIC_H
