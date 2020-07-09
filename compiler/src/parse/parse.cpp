//获得语法树
#include <cstring>
#include "parse.h"
#include "Grammar.h"

void displayAST(Nodebase *root) {
    switch (root->nodetype) {
        case ND_SEMICOLON:
        case ND_LEFTPAR:
        case ND_RIGHTPAR:
        case ND_COMMA:
        case ND_SHARP:
        case ND_TLIDE:
        case ND_LEFTBRKT:
        case ND_RIGHTBRKT:
        case ND_LEFTBRACE:
        case ND_RIGHTBRACE:
        case ND_DOT:
        case ND_COLON:
        case ND_LT:
        case ND_GT:
        case ND_ASSIGN:
        case ND_EX:
        case ND_AND:
        case ND_OR:
        case ND_ADD:
        case ND_SUB:
        case ND_MUL:
        case ND_DIV:
        case ND_MOD:
        case ND_XOR:

        case ND_QUESTIONMARK:;
    }
}



/*

Type *type_void = new Type{ VOID, 0};
Type *type_bool = new Type{ BOOL, 1};
Type *type_char = new Type{ CHAR, 1};
Type *type_number = new Type{ NUMBER, 8};
Type *type_string = new Type{ STRING , 8,type_char};


//number a = 3;
//number main(){
//   number  b =3;
//   return a+3*b;
// }
Nodebase *get_AST(){
    Token *a = new Token;
    auto i = new char[8];
    static auto s1 = "number";
    strcpy(i,s1);
    a->Name = i;

    Token *b = new Token;
    i = new char[8];
    static auto s2 = "a";
    strcpy(i,s2);
    b->Name = i;

    Token *c = new Token;
    i = new char[8];
    static auto s3 = "=";
    strcpy(i,s3);
    c->Name = i;

    Token *d = new Token;
    i = new char[8];
    static auto s4 = "3";
    strcpy(i,s4);
    d->Name = i;

    Token *e = new Token;
    i = new char[8];
    static auto s5 = ";";
    strcpy(i,s5);
    e->Name = i;

    Nodebase *program = new Nodebase(ND_PROG,"Program start!");

    program->stmts.push_back(new Declaration_node(ND_VARDEF, a, type_number));


    auto _equ =
        new Expression_Statement_node(ND_ASSIGN,c);

    _equ->lhs = new Expression_Statement_node(ND_VARREF,b);

    _equ->rhs = new Expression_Statement_node(ND_NUM,d);

    program->stmts.push_back(_equ);

    Token *f = new Token;
    i = new char[8];
    strcpy(i,"b");
    b->Name = i;


    Token *g = new Token;//main
    i = new char[8];
    strcpy(i,"main");
    g->Name = i;

    Token *h = new Token;//+
    i = new char[8];
    strcpy(i,"+");
    h->Name = i;


    Token *j = new Token;//return
    i = new char[8];
    strcpy(i, "return");
    j->Name = i;

    Token *k = new Token;//+
    i = new char[8];
    strcpy(i, "*");
    k->Name = i;


    auto _main = new Declaration_node(ND_FUNC, g, "number");
    _main->stmts.push_back(new Declaration_node(ND_VARDEF, f, "number"));
    auto _equ2 = new Expression_Statement_node(ND_ASSIGN, c);
    _equ2->lhs = new Expression_Statement_node(ND_VARREF, f);
    _equ2->rhs = new Expression_Statement_node(ND_NUM, d);
    _main->stmts.push_back(_equ2);

    auto _ret = new Expression_Statement_node(ND_RETURN, j, type_void);
    auto _3 = new Expression_Statement_node(ND_ADD, k, type_number);
    _3->lhs = new Expression_Statement_node(ND_VARREF, b, type_number);
    _3->rhs = new Expression_Statement_node(ND_MUL, k, type_number);
    _3->rhs->lhs = new Expression_Statement_node(ND_NUM, d, type_number);
    _3->rhs->rhs = new Expression_Statement_node(ND_VARREF,f,type_number);
    _ret->returnval = _3;
    _main->stmts.push_back(_ret);

    program->stmts.push_back(_main);

    /*     program
     *     /  \    \
     *    /    \      \
     *    |     |         \
     *   /       \         main
     *_number   _equ     /    \     \
     *    |     /  \   number equ    _return
     *    a    /   |    |     / \      |
     *       _a    3    b   b    3     +
     *                               /  \
     *                              a    *
     *                                  /  \
     *                                 3    b
     *
     *//*

    return program;
}
*/