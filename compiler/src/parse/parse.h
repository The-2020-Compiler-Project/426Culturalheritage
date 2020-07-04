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

    //function
    class Type *returning;
};

class Var{
    char * name;
    Data_type *type;
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

class Token;//TODO: 删去前置声明
//节点类型
enum NODE_TYPE{
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
  ND_DOT,       // Struct member access
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LE,        // <=
  ND_LOGAND,    // &&
  ND_LOGOR,     // ||
  ND_SHL,       // <<
  ND_SHR,       // >>
  ND_MOD,       // %
  ND_RETURN,    // "return"
  ND_CALL,      // Function call
  ND_FUNC,      // Function definition
  ND_COMP_STMT, // Compound statement
  ND_EXPR_STMT, // Expression statement
  ND_NULL,      // Null statement
};

//变量类型

class Nodebase {
    NODE_TYPE nodetype;
    Nodebase * next;//多叉树的下个兄弟
    Token *tok;//
public:
    virtual ~Nodebase();
};

//声明节点
class Declaration_node : public Nodebase {
    Data_type type;

    //function
    Nodebase *parm;
    Nodebase *funtion_body;
};

//表达式和语句节点
class Expression_Statement_node : public Nodebase {
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
    long long val;//值
};
#endif
