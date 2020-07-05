//获得语法树
#include "parse.h"
#include <cstring>
#include "../lexical/lex.h"

static auto s1 = "number";
static auto s2 = "a";
static auto s3 = "=";
static auto s4 = "3";
static auto s5 = ";";

//number a = 3;
Nodebase *get_AST(){
    Token *a = new Token;
    a->Name = new char[8];
    strcpy(a->Name,s1);

    Token *b = new Token;
    b->Name = new char[8];
    strcpy(b->Name,s2);

    Token *c = new Token;
    c->Name = new char[8];
    strcpy(c->Name,s3);

    Token *d = new Token;
    d->Name = new char[8];
    strcpy(d->Name,s4);

    Token *e = new Token;
    e->Name = new char[8];
    strcpy(e->Name,s5);
    Nodebase *program = new Nodebase(ND_PROG,nullptr,nullptr);

    auto _number = program->next = new Declaration_node(NUMBER,a);

    auto _equ = 
        new Expression_Statement_node(ND_ASSIGN,c);

    auto _a = _equ->lhs = new Expression_Statement_node(ND_VARREF,b);

    auto _3 = _equ->rhs = new Expression_Statement_node(ND_NUM,d);
                                                            
    _number->next = _equ;
    /*   program -> _number -> _equ
     *                          /  \
     *                        _a    3 
     */
    return program;
}
