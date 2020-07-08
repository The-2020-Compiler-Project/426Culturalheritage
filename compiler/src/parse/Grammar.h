//
// Created by Monarcaa on 2020/7/5.
//

#ifndef COMPILER_GRAMMAR_H
#define COMPILER_GRAMMAR_H

#include "lex.h"
#include "parse.h"

Nodebase *syntax();

void Program();//语法分析
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

void FunctionDecl(Type *returning, Token *tok);

int IfType();

void ControlStream();

void Expression();

void IfExpression();

void WhileExpression();

void IfParameter();

void addParameter(Declaration_node *func);

void variableDef(Type *t, Token *var, Nodebase *);

#endif //COMPILER_GRAMMAR_H
