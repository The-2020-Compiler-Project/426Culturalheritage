//
// Created by Monarcaa on 2020/7/5.
//

#ifndef COMPILER_GRAMMAR_H
#define COMPILER_GRAMMAR_H

#include "lex.h"
#include "parse.h"

Nodebase *syntax();

void Program(Nodebase *);//语法分析
void VariableSetting();//变量声明
int ExceptStructOrArray();//判断是否为除struct以外的类型
int VariableName();//变量名
void StructNaming();//struct声明
void NumberSetting();

void StringSetting();

void CharSetting();

void BoolSetting();

void StructSetting();

void ArraySetting();

void PointSetting();

void FunctionDecl(Type *returning, Token *tok, Nodebase *env);

int IfType();

Nodebase *ControlStream(Nodebase *);

void Expression();

void IfExpression();

void WhileExpression();

void IfParameter();

void addParameter(Declaration_node *func);

void variableDef(Type *t, Token *var, Nodebase *);


Expression_Statement_node *expr();

Expression_Statement_node *
new_assign_eq(NODE_TYPE op, Token *top, Expression_Statement_node *lhs, Expression_Statement_node *rhs);

Expression_Statement_node *assign();

#endif //COMPILER_GRAMMAR_H
