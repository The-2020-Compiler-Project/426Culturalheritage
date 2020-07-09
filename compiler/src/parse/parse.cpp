//获得语法树
#include <cstring>
#include <iostream>
#include "parse.h"
#include "Grammar.h"
#include "error_diagnostics.h"

using namespace std;

void binaryDisplay(Nodebase *root, int depth) {
    auto i = dynamic_cast<Expression_Statement_node *>(root);
    displayAST(i->lhs);
    displayAST(i->rhs);
}

void displayAST(Nodebase *root, int depth) {
    switch (root->nodetype) {
        //trinary
        case ND_QUESTIONMARK:
            //binary operator
        case ND_SEMICOLON:
            cout << "SEMICOLON" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_LEFTPAR:
            cout << "LEFTPAR" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_RIGHTPAR:
            cout << "RIGHTPAR" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_COMMA:
            cout << "COMMA" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_SHARP:
            cout << "SHARP" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_TLIDE:
            cout << "TLIDE" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_LEFTBRKT:
            cout << "LEFTBRKT" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_RIGHTBRKT:
            cout << "RIGHTBRKT" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_LEFTBRACE:
            cout << "LEFTBRACE" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_RIGHTBRACE:
            cout << "RIGHTBRACE" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_DOT:
            cout << "DOT" << endl;
            cout << dynamic_cast<Expression_Statement_node *>(root)->getName() << "  "
                 << dynamic_cast<Expression_Statement_node *>(root)->struc->getName() << "  "
                 << dynamic_cast<Expression_Statement_node *>(root)->field;
            break;
        case ND_COLON:
            cout << "COLON" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_LT:
            cout << "LT" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_GT:
            cout << "GT" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_ASSIGN:
            cout << "ASSIGN" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_AND:
            cout << "AND" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_OR:
            cout << "OR" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_ADD:
            cout << "ADD" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_SUB:
            cout << "SUB" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_MUL:
            cout << "MUL" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_DIV:
            cout << "DIV" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_MOD:
            cout << "MOD" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_XOR :
            cout << "XOR " << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_EQ:
            cout << "EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_NE:
            cout << "NE" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_LE:
            cout << "LE" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_GE:
            cout << "GE" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_LOGOR:
            cout << "LOGOR" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_LOGAND:
            cout << "LOGAND" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_SHL:
            cout << "SHL" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_SHR:
            cout << "SHR" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_MUL_EQ:
            cout << "MUL_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_DIV_EQ:
            cout << "DIV_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_MOD_EQ:
            cout << "MOD_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_ADD_EQ:
            cout << "ADD_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_SUB_EQ:
            cout << "SUB_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_SHL_EQ:
            cout << "SHL_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_SHR_EQ:
            cout << "SHR_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_AND_EQ:
            cout << "AND_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_XOR_EQ:
            cout << "XOR_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;
        case ND_OR_EQ:
            cout << "OR_EQ" << endl;
            binaryDisplay(root, depth + 1);
            break;

            //unary
        case ND_EX:;
        case ND_INC:;
        case ND_DEC:;
        case ND_ADDR:
        case ND_DEREF:
            break;

            //direct_to_stmts
        case ND_FUNC:
        case ND_COMP_STMT:
        case ND_PROG:
            for (auto x:root->stmts) {
                displayAST(x);
            }
            break;

        case ND_NUM:
            cout << root->getName() << endl;
            break;
        case ND_CHAR:
            cout << root->getName() << endl;
            break;
        case ND_STR:
            cout << root->getName() << endl;
            break;
        case ND_STRUCT:
            cout << root->getName() << endl;
            break;
        case ND_TYPEDECL:
            cout << root->getName() << endl;
            break;
        case ND_VARDEF:
            cout << root->getName() << endl;
            break;
        case ND_VARREF:
            cout << root->getName() << endl;
            break;
        case ND_CAST:
            break;
        case ND_IF:
            break;
        case ND_FOR:
            break;
        case ND_DO_WHILE:
            break;
        case ND_SWITCH:
            break;
        case ND_CASE:
            break;
        case ND_BREAK:
            break;
        case ND_CONTINUE:
            break;
        case ND_RETURN:
            break;
        case ND_CALL:
            cout << root->getName() << endl;
            break;
        default:
            token_error(root->tok, "Bad node in tree");
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