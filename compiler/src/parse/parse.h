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
#include <map>
#include "../lexical/lex.h"
//节点类型
enum NODE_TYPE{
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



enum Data_type{
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

class Type{
    Data_type data_type;
    int size;

    //Pointer
    class Type *ptr_to;

    //Array
    class Type *array_of;
    int len;

    //Struct
    std::map<std::string,class Type *> members;
    int offset;//sizeof(struct sth)
    //int alignof;

    //function
    class Type *returning;
};

class Var{
    char * name;
    Data_type *type;
    char *data;
};

class Func{
    char *name;
    class Node *node;
};

class Env{
    std::map<std::string, Var> vars;//变量声明
    std::map<std::string, Func> funcs;//内嵌函数
    Env *prev;
};

//符号表
class table{



};





//抽象语法树
//变量类型


//语法树节点基类
class Nodebase {
    public:
        Nodebase * next;//多叉树的下个兄弟
        NODE_TYPE nodetype;
        Token *tok;

        Nodebase(NODE_TYPE nt,Token *tk,Nodebase *n = nullptr):
            nodetype(nt),
            next(n),
            tok(tk){};
        Nodebase(const Nodebase &Nodebase) = delete;
        Nodebase(Nodebase &&Nodebase) = delete;
        Nodebase operator==(const Nodebase &Nodebase) = delete;
        Nodebase operator==(Nodebase &&Nodebase) = delete;
        //void setNext(Nodebase *n){next = n;}
        //Nodebase *Next() const {return next;}
        //Token *token() const {return tok;}

        virtual ~Nodebase();
};

//声明节点
class Declaration_node : public Nodebase {
    public:
        Declaration_node(Data_type dt, Token *tk, Nodebase *pam = nullptr,Nodebase *body = nullptr):
            Nodebase(ND_VARDEF,tk),
            type(dt),
            parm(pam),
            funtion_body(body){}
        Data_type type;

        //function
        Nodebase *parm;
        Nodebase *funtion_body;
};

//表达式和语句节点
class Expression_Statement_node : public Nodebase {
public:
    Expression_Statement_node(NODE_TYPE tp,Token *tk):
        Nodebase(tp,tk){}
    Nodebase *lhs;//left-hand
    Nodebase *rhs;//right-hand

    //if (cond) body else else_body
    //for (init; cond;inc)
    //while (cond) body
    //do body while(cond)
    //switch (cond) body
    //case val : body
    Nodebase *cond;
    Nodebase *body;
    Nodebase *else_body;
    Nodebase *init;
    Nodebase *inc;

    Var *var;//变量
    long double val;//值
};

#endif
