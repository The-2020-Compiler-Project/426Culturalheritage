//Define the nodes of the AST
/** 
 * \file ast.h
 * \brief defination of AST
 * \author emon100
 *
 */
#ifndef _SRC_AST_AST_H
#define _SRC_AST_AST_H

#include <string>
#include <vector>
#include <map>
#include "../lexical/lex.h"

//节点类型
enum NODE_TYPE {
    ND_SEMICOLON = ';',
    ND_LEFTPAR = '(',
    ND_RIGHTPAR = ')',
    ND_COMMA = ',',
    ND_SHARP = '#',
    ND_TLIDE = '~',
    ND_LEFTBRKT = '[',
    ND_RIGHTBRKT = ']',
    ND_LEFTBRACE = '{',
    ND_RIGHTBRACE = '}',
    ND_DOT = '.',         // Struct member access
    ND_QUESTIONMARK = '?',
    ND_COLON = ':',
    ND_LT = '<',
    ND_GT = '>',
    ND_ASSIGN = '=',
    ND_EX = '!',
    ND_AND = '&',
    ND_OR = '|',
    ND_ADD = '+',
    ND_SUB = '-',
    ND_MUL = '*',
    ND_DIV = '/',
    ND_MOD = '%',
    ND_XOR = '^',
    ND_NUM = 256, // Number literal
    ND_STRUCT,    // Struct
    ND_DECL,      // declaration
    ND_VARDEF,    // Variable definition
    ND_VARREF,    // Variable reference
    ND_CAST,      // Cast
    ND_IF,        // "if"
    ND_FOR,       // "for"
    ND_DO_WHILE,  // do ... while
    ND_SWITCH,    // switch
    ND_CASE,      // case
    ND_BREAK,     // break
    ND_CONTINUE,  // continue
    ND_ADDR,      // address-of operator ("&")
    ND_DEREF,     // pointer dereference ("*")
    ND_EQ,         // ==
    ND_NE,         // !=
    ND_LE,         // <=
    ND_GE,         // >=
    ND_LOGOR,      // ||
    ND_LOGAND,     // &&
    ND_SHL,        // <<
    ND_SHR,        // >>
    ND_INC,        // ++
    ND_DEC,        // --
    ND_MUL_EQ,     // *=
    ND_DIV_EQ,     // /=
    ND_MOD_EQ,     // %=
    ND_ADD_EQ,     // +=
    ND_SUB_EQ,     // -=
    ND_SHL_EQ,     // <<=
    ND_SHR_EQ,     // >>=
    ND_AND_EQ,     // &=
    ND_XOR_EQ,     // ^=
    ND_OR_EQ,      // |=
    ND_RETURN,    // "return"
    ND_CALL,      // Function call
    ND_FUNC,      // Function definition
    ND_COMP_STMT, // Compound statement
    ND_EXPR_STMT, // Expression statement
    ND_NULL,      // Null statement
    ND_PROG
};

//一些基本数据类型的种类
enum Data_type {
    VOID,
    NUMBER,
    CHAR,
    STRING,
    BOOL,
    PTR,
    STRUCT,
    ARRAY,
    FUNC
};

//类型项，记录了类型信息
struct Type {
    Type(Data_type dt, int sz, Type *ptr = nullptr) :
            data_type(dt), size(sz), ptr_to(ptr) {}

    Data_type data_type;
    int size;

    //Pointer
    Type *ptr_to;

    //Array
    Type *array_of;
    int len;

    //Struct
    std::map<std::string, Type *> members;
    int offset;//sizeof(struct sth)
    //int alignof;

    //function
    Type *returning;
};


class Var{
    char * name;
    Data_type *type;
    char *data;
};

struct Func {
    char *name;

    class Node *node;
};

/*
struct Env{
    Env(Env *pv):prev(pv){}
   //std::map<std::string, Var> vars;//变量声明//TODO:
    std::map<std::string, Func> funcs;//内嵌函数
    Env *prev;
};
 */



//抽象语法树
//变量类型


//语法树节点基类
class Nodebase {
public:
    //Nodebase * next;//多叉树的下个兄弟
    std::vector<Nodebase *> stmts;
    NODE_TYPE nodetype;
    Type *type;
    Token *tok;

    Nodebase(NODE_TYPE nt, Token *tk, Type *t = nullptr) :
            type(t),
            nodetype(nt),
            tok(tk) {}

    Nodebase(NODE_TYPE nt, const char *Name, Type *t = nullptr) :
            type(t),
            nodetype(nt) {
        tok = new Token;
        tok->Name = Name;
    };

    //Nodebase(const Nodebase &Nodebase) = delete;
    //Nodebase(Nodebase &&Nodebase) = delete;
    //Nodebase operator==(const Nodebase &Nodebase) = delete;
    //Nodebase operator==(Nodebase &&Nodebase) = delete;
    //void setNext(Nodebase *n){next = n;}
    //Nodebase *Next() const {return next;}
    //Token *token() const {return tok;}
    const char *getName() const { return tok->Name; }

    virtual ~Nodebase() = default;
};

//声明节点
class Declaration_node : public Nodebase {
public:
    std::vector<char *>parms;//形参
    Declaration_node(NODE_TYPE nt,Token *tk, Type *dt = nullptr, Nodebase *pam = nullptr, Nodebase *body = nullptr) :
            Nodebase(nt, tk, dt)
            {}

    //函数声明或调用
    Type *ftype;//函数返回值类型
    std::vector<char *>args;//实参
    Nodebase *fptr;//函数指针
    std::vector<char *>localvars;
    Nodebase *function_body;

    //变量声明
    Nodebase *declvar;
    std::vector<char *>declinit;


    //function
};

//表达式和语句节点
class Expression_Statement_node : public Nodebase {
public:
    Expression_Statement_node(NODE_TYPE tp, Token *tk, Type *dt = nullptr) :
            Nodebase(tp, tk, dt){}
    Data_type type;

    union{
        long double val;//number, char, bool的值
        struct  {// 字符串的值
            char *sval;
        };
        struct {//二元运算
            Expression_Statement_node *lhs;//left-hand
            Expression_Statement_node *rhs;//right-hand
        };
        struct {//一元
            Expression_Statement_node *operand;
        };
        //if (cond) body else else_body
        //for (init; cond;inc)
        //while (cond) ody
        //do body while(cond)
        //switch (cond) body
        //case val : body
        struct {
            Nodebase *cond;
            Nodebase *body;
            Nodebase *else_body;
            Nodebase *init;
            Nodebase *inc;
        };
        Nodebase *returnval;//返回值
        struct {//struct
            struct Node *struc;
            char *field;
            Type *fieldtype;
        };
    };

    ~Expression_Statement_node() = default;
};


Nodebase *get_AST();

#endif
