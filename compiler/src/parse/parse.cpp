//获得语法树
#include <cstring>
#include <iostream>
#include "parse.h"
#include "Grammar.h"
#include "error_diagnostics.h"

using namespace std;

void binaryDisplay(Nodebase *root, int depth, const char *type) {
    auto i = dynamic_cast<Expression_Statement_node *>(root);
    displayAST(i->lhs, depth + 1);
    int t1 = depth;
    while (t1-- + 2) {
        cout << " ";
    }
    cout << "/\n";
    t1 = depth;
    while (--t1) {
        cout << " ";
    }
    cout << type << endl;
    t1 = depth;
    while (t1-- + 2) {
        cout << " ";
    }
    cout << "\\\n";
    displayAST(i->rhs, depth + 1);
}

void displayAST(Nodebase *root, int depth) {
    switch (root->nodetype) {
        //trinary
        case ND_QUESTIONMARK:
            //binary operator
        case ND_SEMICOLON:
            binaryDisplay(root, depth + 1, "SEMICOLON");

            break;
        case ND_LEFTPAR:
            binaryDisplay(root, depth + 1, "LEFTPAR");

            break;
        case ND_RIGHTPAR:
            binaryDisplay(root, depth + 1, "RIGHTPAR");

            break;
        case ND_COMMA:
            binaryDisplay(root, depth + 1, "COMMA");

            break;
        case ND_SHARP:
            binaryDisplay(root, depth + 1, "SHARP");

            break;
        case ND_TLIDE:
            binaryDisplay(root, depth + 1, "TLIDE");

            break;
        case ND_LEFTBRKT:
            binaryDisplay(root, depth + 1, "LEFTBRKT");

            break;
        case ND_RIGHTBRKT:
            binaryDisplay(root, depth + 1, "RIGHTBRKT");

            break;
        case ND_LEFTBRACE:
            binaryDisplay(root, depth + 1, "LEFTBRACE");

            break;
        case ND_RIGHTBRACE:
            binaryDisplay(root, depth + 1, "RIGHTBRACE");

            break;
        case ND_DOT:
            cout << "DOT"
                 << "  "
                 << "  "
                 << dynamic_cast<Expression_Statement_node *>(root)->field;
            break;
        case ND_COLON:
            binaryDisplay(root, depth + 1, "COLON");

            break;
        case ND_LT:
            binaryDisplay(root, depth + 1, "LT");

            break;
        case ND_GT:
            binaryDisplay(root, depth + 1, "GT");

            break;
        case ND_ASSIGN:
            binaryDisplay(root, depth + 1, "ASSIGN");

            break;
        case ND_AND:
            binaryDisplay(root, depth + 1, "AND");

            break;
        case ND_OR:
            binaryDisplay(root, depth + 1, "OR");

            break;
        case ND_ADD:
            binaryDisplay(root, depth + 1, "ADD");

            break;
        case ND_SUB:
            binaryDisplay(root, depth + 1, "SUB");

            break;
        case ND_MUL:
            binaryDisplay(root, depth + 1, "MUL");

            break;
        case ND_DIV:
            binaryDisplay(root, depth + 1, "DIV");

            break;
        case ND_MOD:
            binaryDisplay(root, depth + 1, "MOD");

            break;
        case ND_XOR :
            binaryDisplay(root, depth + 1, "XOR ");

            break;
        case ND_EQ:
            binaryDisplay(root, depth + 1, "EQ");

            break;
        case ND_NE:
            binaryDisplay(root, depth + 1, "NE");

            break;
        case ND_LE:
            binaryDisplay(root, depth + 1, "LE");

            break;
        case ND_GE:
            binaryDisplay(root, depth + 1, "GE");

            break;
        case ND_LOGOR:
            binaryDisplay(root, depth + 1, "LOGOR");

            break;
        case ND_LOGAND:
            binaryDisplay(root, depth + 1, "LOGAND");

            break;
        case ND_SHL:
            binaryDisplay(root, depth + 1, "SHL");

            break;
        case ND_SHR:
            binaryDisplay(root, depth + 1, "SHR");

            break;
        case ND_MUL_EQ:
            binaryDisplay(root, depth + 1, "MUL_EQ");

            break;
        case ND_DIV_EQ:
            binaryDisplay(root, depth + 1, "DIV_EQ");

            break;
        case ND_MOD_EQ:
            binaryDisplay(root, depth + 1, "MOD_EQ");

            break;
        case ND_ADD_EQ:
            binaryDisplay(root, depth + 1, "ADD_EQ");

            break;
        case ND_SUB_EQ:
            binaryDisplay(root, depth + 1, "SUB_EQ");

            break;
        case ND_SHL_EQ:
            binaryDisplay(root, depth + 1, "SHL_EQ");

            break;
        case ND_SHR_EQ:
            binaryDisplay(root, depth + 1, "SHR_EQ");

            break;
        case ND_AND_EQ:
            binaryDisplay(root, depth + 1, "AND_EQ");

            break;
        case ND_XOR_EQ:
            binaryDisplay(root, depth + 1, "XOR_EQ");

            break;
        case ND_OR_EQ:
            binaryDisplay(root, depth + 1, "OR_EQ");

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
        case ND_CHAR:
        case ND_STR:
        case ND_STRUCT:
        case ND_TYPEDECL:
        case ND_VARDEF:
        case ND_VARREF: {
            int t1 = depth;
            while (t1-- + 2) {
                cout << " ";
            }
            cout << root->getName() << endl;
        }
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