#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include "lex.h"
#include "Grammar.h"
#include "error_diagnostics.h"

using namespace std;
static list<Token> Token_list;

static std::unordered_map<string, Type *> types{
        {{"void"},   new Type{VOID, 0}},
        {{"bool"},   new Type{BOOL, 1}},
        {{"char"},   new Type{CHAR, 1}},
        {{"number"}, new Type{NUMBER, 8}},
        {{"string"}, new Type{STRING, 8}},
};
static Token currentToken;
#define currentTokenAddr &Token_list.back()
static Token tempToken;

Expression_Statement_node *
new_binop(NODE_TYPE nd, Token *t, Expression_Statement_node *lhs, Expression_Statement_node *rhs) {
    auto *node = new Expression_Statement_node(nd, t);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Type *getType(const Token &tok) {
    auto find = types.find(tok.Name);
    return (find == types.end() ? nullptr : (*find).second);
}

Type *getType(const char *name) {
    auto find = types.find(name);
    return (find == types.end() ? nullptr : (*find).second);
}

const Token &updateCurrentToken() {
    currentToken = get_token();
    Token_list.push_back(currentToken);
    return Token_list.back();
}

bool consume(Token_type ty) {
    if (currentToken.token_type != ty) {
        return false;
    }
    updateCurrentToken();
    return true;
}

void expect(Token_type ty, const char *msg = nullptr) {
    if (currentToken.token_type == ty) {
        updateCurrentToken();
        return;
    } else {//badToken
        token_error(currentTokenAddr, msg ? msg : "Unexpected Token");
    }
}

Type *ptr_to(Type *tp) {
    Type *ptr = new Type(PTR, 8, tp);
    return ptr;
}

Expression_Statement_node *function_call(Token *t) {
    auto functy = getType(*t);

    if (functy == nullptr) {
        token_error(t, "undefined function");//already end here;
        return nullptr;
    } else {
        auto *node = new Expression_Statement_node(ND_CALL, t, functy->returning);
        while (!consume(TK_RIGHTPAR)) {
            if (!node->args.empty())
                expect(TK_COMMA);
            node->args.push_back(assign());
        }
        return node;
    }
}


Expression_Statement_node *new_expr(NODE_TYPE op, Token *t, Nodebase *expr) {
    auto *node = new Expression_Statement_node(op, t);
    node->returnval = expr;
    return node;
}


Expression_Statement_node *primary() {
    Token *t = currentTokenAddr;
    updateCurrentToken();

    if (t->token_type == '(') {
        auto node = expr();
        expect(TK_RIGHTPAR);
        return node;
    }

    if (t->token_type == TK_NUM)
        return new Expression_Statement_node(ND_NUM, t, getType("number"));//new_int_node(t->val, t);

    if (t->token_type == TK_CHAR) {
        return new Expression_Statement_node(ND_CHAR, t, getType("char"));//new_int_node(t->val, t);
    }

    if (t->token_type == TK_STR)
        return new Expression_Statement_node(ND_STR, t, getType("string"));//string_literal(t);

    if (t->token_type == TK_IDENT) {
        if (consume(TK_LEFTPAR))
            return function_call(t);
        return new Expression_Statement_node(ND_VARREF, t);
    }

    token_error(t, "primary expression expected");//already failed here.
    return nullptr;
}

Expression_Statement_node *mul();


Expression_Statement_node *postfix() {//后缀目前只支持点，箭头，下标
    Expression_Statement_node *lhs = primary();
    for (;;) {
        Token *t = currentTokenAddr;

        /* TODO 补充回来
            if (consume(TK_INC)) {
                lhs = new_post_inc(t, lhs, 1);
                continue;
            }

            if (consume(TK_DEC)) {
                lhs = new_post_inc(t, lhs, -1);
                continue;
            }
            */

        if (consume(TK_DOT)) {
            lhs = new_expr(ND_DOT, currentTokenAddr, lhs);
            continue;
        }

        if (consume(TK_ARROW)) {
            lhs = new_expr(ND_DOT, currentTokenAddr, new_expr(ND_DEREF, t, lhs));
            continue;
        }

        if (consume(TK_LEFTBRKT)) {
            Expression_Statement_node *node = new_binop(ND_ADD, t, lhs, assign());
            lhs = new_expr(ND_DEREF, t, node);
            expect(TK_RIGHTBRKT);
            continue;
        }
        return lhs;
    }
}


Expression_Statement_node *unary() {
    Token *t = currentTokenAddr;

    Token *zero;
    if (consume(TK_SUB)) {
        zero = new Token();
        zero->token_type = TK_NUM;
        zero->Name = "0";
        return new_binop(ND_SUB, t, new Expression_Statement_node(ND_NUM, zero, getType("number")), unary());
    }
    if (consume(TK_MUL))//*
        return new_expr(ND_DEREF, t, unary());
    if (consume(TK_AND))//&
        return new_expr(ND_ADDR, t, unary());
    if (consume(TK_EX))// !
        return new_expr(ND_EX, t, unary());
    if (consume(TK_TLIDE))//~
        return new_expr(ND_TLIDE, t, unary());
    /*
    if (consume(TK_SIZEOF))
        return new_int_node(get_type(unary())->size, t);
    if (consume(TK_ALIGNOF))
        return new_int_node(get_type(unary())->align, t);
        */
    if (consume(TK_INC)) {
        zero = new Token();
        zero->token_type = TK_NUM;
        zero->Name = "1";
        return new_assign_eq(ND_ADD, t, unary(), new Expression_Statement_node(ND_NUM, zero, getType("number")));
    }

    if (consume(TK_DEC)) {
        zero = new Token();
        zero->token_type = TK_NUM;
        zero->Name = "1";
        return new_assign_eq(ND_SUB, t, unary(), new Expression_Statement_node(ND_NUM, zero, getType("number")));
    }
    return postfix();
}

Expression_Statement_node *mul() {
    Expression_Statement_node *lhs = unary();
    for (;;) {
        Token *t = currentTokenAddr;
        if (consume(TK_MUL))
            lhs = new_binop(ND_MUL, t, lhs, unary());
        else if (consume(TK_DIV))
            lhs = new_binop(ND_DIV, t, lhs, unary());
        else if (consume(TK_MOD))
            lhs = new_binop(ND_MOD, t, lhs, unary());
        else
            return lhs;
    }
}

Expression_Statement_node *add() {
    Expression_Statement_node *lhs = mul();
    for (;;) {
        Token *t = currentTokenAddr;
        if (consume(TK_ADD))
            lhs = new_binop(ND_ADD, t, lhs, mul());
        else if (consume(TK_SUB))
            lhs = new_binop(ND_SUB, t, lhs, mul());
        else
            return lhs;
    }
}

Expression_Statement_node *shift() {
    Expression_Statement_node *lhs = add();
    for (;;) {
        Token *t = currentTokenAddr;
        if (consume(TK_SHL))
            lhs = new_binop(ND_SHL, t, lhs, add());
        else if (consume(TK_SHR))
            lhs = new_binop(ND_SHR, t, lhs, add());
        else
            return lhs;
    }
}

Expression_Statement_node *relational() {
    Expression_Statement_node *lhs = shift();
    for (;;) {
        Token *t = currentTokenAddr;
        if (consume(TK_LT))
            lhs = new_binop(ND_LT, t, lhs, shift());
        else if (consume(TK_GT))
            lhs = new_binop(ND_GT, t, shift(), lhs);
        else if (consume(TK_LE))
            lhs = new_binop(ND_LE, t, lhs, shift());
        else if (consume(TK_GE))
            lhs = new_binop(ND_LE, t, shift(), lhs);
        else
            return lhs;
    }
}

Expression_Statement_node *equality() {
    Expression_Statement_node *lhs = relational();
    for (;;) {
        Token *t = currentTokenAddr;
        if (consume(TK_EQ))
            lhs = new_binop(ND_EQ, t, lhs, relational());
        else if (consume(TK_NE))
            lhs = new_binop(ND_NE, t, lhs, relational());
        else
            return lhs;
    }
}

Expression_Statement_node *bit_and() {
    Expression_Statement_node *lhs = equality();
    while (consume(TK_AND)) {
        Token *t = currentTokenAddr;
        lhs = new_binop(ND_AND, t, lhs, equality());
    }
    return lhs;
}

Expression_Statement_node *bit_xor() {
    Expression_Statement_node *lhs = ::bit_and();
    while (consume(TK_XOR)) {
        Token *t = currentTokenAddr;
        lhs = new_binop(ND_XOR, t, lhs, ::bit_and());
    }
    return lhs;
}

Expression_Statement_node *bit_or() {
    Expression_Statement_node *lhs = ::bit_xor();
    while (consume(TK_OR)) {
        Token *t = currentTokenAddr;
        lhs = new_binop(ND_OR, t, lhs, ::bit_xor());
    }
    return lhs;
}

Expression_Statement_node *logand() {
    Expression_Statement_node *lhs = ::bit_or();
    while (consume(TK_LOGAND)) {
        Token *t = currentTokenAddr;
        lhs = new_binop(ND_LOGAND, t, lhs, ::bit_or());
    }
    return lhs;
}

Expression_Statement_node *logor() {
    Expression_Statement_node *lhs = logand();
    while (consume(TK_LOGOR)) {
        Token *t = currentTokenAddr;
        lhs = new_binop(ND_LOGOR, t, lhs, logand());
    }
    return lhs;
}

Expression_Statement_node *conditional() {
    Expression_Statement_node *cond = logor();
    currentTokenAddr;
    if (!consume(TK_QUESTIONMARK))
        return cond;

    auto *node = new Expression_Statement_node(ND_QUESTIONMARK, &(*(++Token_list.rbegin())));
    node->cond = cond;
    node->body = expr();
    expect(TK_COLON);
    node->else_body = ::conditional();
    return node;
}

// `x op= y` where x is of type T is compiled as
// ` ( x = x op y )

Expression_Statement_node *
new_assign_eq(NODE_TYPE op, Token *top, Expression_Statement_node *lhs, Expression_Statement_node *rhs) {
    auto stringassign = "=";
    vector<Nodebase *> v;//这个语句块
    Token *tx = lhs->tok;
    Token *ty = rhs->tok;
    auto *assign = new Token;
    assign->token_type = TK_ASSIGN;
    assign->Name = stringassign;
    auto add = new Expression_Statement_node(op, top);
    add->lhs = new Expression_Statement_node(ND_VARREF, tx);
    add->rhs = new Expression_Statement_node(ND_VARREF, ty);
    return new_binop(ND_ASSIGN, assign, new Expression_Statement_node(ND_VARREF, tx), add);
}

Expression_Statement_node *assign() {
    Expression_Statement_node *lhs = ::conditional();
    Token *t = currentTokenAddr;

    if (consume(TK_ASSIGN))
        return new_binop(ND_ASSIGN, t, lhs, assign());
    if (consume(TK_MUL_EQ))
        return new_assign_eq(ND_MUL, t, lhs, assign());
    if (consume(TK_DIV_EQ))
        return new_assign_eq(ND_DIV, t, lhs, assign());
    if (consume(TK_MOD_EQ))
        return new_assign_eq(ND_MOD, t, lhs, assign());
    if (consume(TK_ADD_EQ))
        return new_assign_eq(ND_ADD, t, lhs, assign());
    if (consume(TK_SUB_EQ))
        return new_assign_eq(ND_SUB, t, lhs, assign());
    if (consume(TK_SHL_EQ))
        return new_assign_eq(ND_SHL, t, lhs, assign());
    if (consume(TK_SHR_EQ))
        return new_assign_eq(ND_SHR, t, lhs, assign());
    if (consume(TK_AND_EQ))
        return new_assign_eq(ND_LOGAND, t, lhs, assign());
    if (consume(TK_XOR_EQ))
        return new_assign_eq(ND_XOR, t, lhs, assign());
    if (consume(TK_OR_EQ))
        return new_assign_eq(ND_OR, t, lhs, assign());
    return lhs;
}

Expression_Statement_node *expr() {
    Expression_Statement_node *lhs = assign();
    Token *t = currentTokenAddr;
    if (!consume(TK_COMMA))
        return lhs;
    return new_binop(ND_COMMA, t, lhs, expr());
    /*
    return new Expression_Statement_node(ND_NUM, currentTokenAddr);
     */
}


Nodebase *variableInit(Type *t, Token *var) {
    auto assignToken = &*(++Token_list.rbegin());
    auto _equ =
            new Expression_Statement_node(ND_ASSIGN, assignToken, t);

    _equ->lhs = new Expression_Statement_node(ND_VARREF, var);

    _equ->rhs = assign();//比逗号优先级高的表达式
    return _equ;
}

void variableDef(Type *t, Token *var, Nodebase *root) {
    root->stmts.push_back(new Declaration_node(ND_VARDEF, var, t));
    if (consume(TK_ASSIGN)) {
        root->stmts.push_back(variableInit(t, var));
    }
}

void Program(Nodebase *program) {
//类型
//标识符
    Type *ty;
    tempToken = currentToken;//int处
    int a = ExceptStructOrArray();//第一个Token的类型 0:struct  1:基本类型 2:数组
    if (a == 2) { //数组类型
        ArraySetting();
    } else if (a == 1) {//基本类型
        ty = getType(tempToken);//TODO 函数时候的类型
        //指针判断应当放在这
        while (consume(TK_MUL)) {
            ty = ptr_to(ty);
        }
        auto tmp = currentTokenAddr;
        //if (consume(TK_IDENT)) {
        expect(TK_IDENT);
        if (consume(TK_LEFTPAR)) {
            FunctionDecl(ty, tmp, program);
        } else {
            variableDef(ty, tmp, program);
            while (consume(TK_COMMA)) {
                tmp = currentTokenAddr;
                if (consume(TK_IDENT)) {
                    variableDef(ty, tmp, program);
                }
            }
            expect(TK_SEMICOLON);
        }
        /*} else if (currentToken.token_type == TK_MUL) {
            while ((updateCurrentToken()).token_type == TK_MUL) {
                continue;
            }
            if (currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
                    FunctionDecl();
                } else {
                    PointSetting();
                }
            } else {
                token_error(currentTokenAddr,"Wrong type!");
                //终止
            }
        }
        else {
            token_error(currentTokenAddr,"Wrong type!1");
            //终止
        }
         */

    } else if (a == 0) {//struct开头//TODO
        ty = getType(currentToken);
        ty = ty ? ty : new Type(STRUCT, -1);
        //指针判断应当放在这
        expect(TK_IDENT);
        while (consume(TK_MUL)) {
            ty = ptr_to(ty);
        }
        auto tmp = currentTokenAddr;
        //if (consume(TK_IDENT)) {
        expect(TK_IDENT);
        if (consume(TK_LEFTPAR)) {
            FunctionDecl(ty, tmp, program);
        } else {
            StructSetting();
        }
        expect(TK_SEMICOLON);

        /*

            if (consume(TK_IDENT)) {
                updateCurrentToken();
                if (currentToken.token_type == TK_LEFTBRACE) {
                    //为{，开始struct声明
                    StructNaming();
                } else if (currentToken.token_type == TK_IDENT) {
                    updateCurrentToken();
                    if (currentToken.token_type == TK_LEFTPAR) {
                        FunctionDecl(nullptr, nullptr, nullptr);
                    } else {
                        StructSetting();
                    }
                } else {
                    token_error(currentTokenAddr, "Wrong StructSetting!2");
                    //终止
                }
            } else if (currentToken.token_type == TK_MUL) {
                while ((updateCurrentToken()).token_type == TK_MUL) {
                    continue;
                }
                if (currentToken.token_type == TK_IDENT) {
                    if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
                        FunctionDecl(nullptr, nullptr, nullptr);
                    } else {
                        PointSetting();
                    }
                }
            } else {
                token_error(currentTokenAddr, "Wrong StructSetting!3");
            }
        } else {
            token_error(currentTokenAddr, "Wrong StructSetting!4");
        }
         */
    }
}

/*
void VariableSetting() {
    //temp
    if (tempToken.token_type == TK_NUMBERTYPE) {
        //number
        NumberSetting();
    } else if (tempToken.token_type == TK_STRINGTYPE) {
        //string
        StringSetting();
    } else if (tempToken.token_type == TK_CHARTYPE) {
        //char
        CharSetting();
    } else if (tempToken.token_type == TK_BOOL) {
        //bool
        BoolSetting();
    } else {
        token_error(currentTokenAddr, "Wrong VariableSetting!5");
        //终止
    }
}

int VariableName() {
    if (currentToken.token_type == TK_IDENT) {
        return 1;
    } else {
        //终止
        token_error(currentTokenAddr, "Wrong VariableName!6");

    }
}

void StructNaming() {
    while ((updateCurrentToken()).token_type != TK_RIGHTBRACE) {
        int a = ExceptStructOrArray();
        tempToken = currentToken;
        if (a == 2) {//数组定义
            ArraySetting();
        } else if (a == 1) {
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
                    IfParameter();
                } else {
                    VariableSetting();
                }
            } else if (currentToken.token_type == TK_MUL) {
                while ((updateCurrentToken()).token_type == TK_MUL) {
                    continue;
                }
                if (currentToken.token_type == TK_IDENT) {
                    if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
                        IfParameter();
                    } else {
                        PointSetting();
                    }
                }
            } else {
                token_error(currentTokenAddr, "Wrong type!7");
                //终止
            }
        } else if (a == 0) {//struct开头
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {//类名
                updateCurrentToken();
                if (currentToken.token_type == TK_IDENT) {//变量名
                    updateCurrentToken();
                    if (currentToken.token_type == TK_LEFTPAR) {
                        IfParameter();
                    } else {
                        StructSetting();
                    }
                } else if (currentToken.token_type == TK_MUL) {
                    while ((updateCurrentToken()).token_type == TK_MUL) {
                        continue;
                    }
                    if (currentToken.token_type == TK_IDENT) {
                        if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
                            IfParameter();
                        } else {
                            PointSetting();
                        }
                    } else {
                        token_error(currentTokenAddr, "Wrong StructNaming!8");
                        //终止
                    }
                } else {
                    token_error(currentTokenAddr, "Wrong StructNaming!9");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong StructNaming!10");
                //终止
            }
        }
    }
    if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong StructNaming!11");
        //终止
    }
}
void NumberSetting() {//赋值
    if (currentToken.token_type == TK_ADD_EQ || currentToken.token_type == TK_SUB_EQ) {
        updateCurrentToken();
        //TODO:贾浩楠,怎么用表达式赋值
        if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong number setting111!");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong number setting112!");
            //终止
        }
    } else if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong number setting101!");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong number setting121!");
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                token_error(currentTokenAddr, "Wrong number setting!12");
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong number setting!13");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong number setting!14");
                //终止
            }
        } else if (currentToken.token_type == TK_SUB_EQ || currentToken.token_type == TK_ADD_EQ) {
            updateCurrentToken();
            if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong number setting111!");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong number setting112!");
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            token_error(currentTokenAddr, "Wrong number setting!15");
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong number setting!103");
        //终止
    }
}

void StringSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_STR || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong String setting101!");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong String setting101!");
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                token_error(currentTokenAddr, "Wrong string setting!16");
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_STR || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong string setting!17");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong string setting!18");
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            token_error(currentTokenAddr, "Wrong string setting!113");
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong string setting!19");
        //终止
    }
}

void CharSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_CHAR || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong char setting102!");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong char setting103!");
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                token_error(currentTokenAddr, "Wrong char setting!20");
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_CHAR || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong char setting!21");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong char setting!22");
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            token_error(currentTokenAddr, "Wrong char setting!112");
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong char setting!23");
        //终止
    }
}

void BoolSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_TRUE || currentToken.token_type == TK_FALSE ||
            currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong bool setting101!");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong bool setting101!");
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                token_error(currentTokenAddr, "Wrong bool setting!24");
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_TRUE || currentToken.token_type == TK_FALSE ||
                currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong bool setting!25");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong bool setting!26");
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            token_error(currentTokenAddr, "Wrong bool setting!111");
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong bool setting!27");
        //终止
    }
}

*/
void ArraySetting() {
    if ((updateCurrentToken()).token_type == TK_LEFTBRKT) {
        if ((updateCurrentToken()).token_type == TK_NUM) {
            if ((updateCurrentToken()).token_type == TK_RIGHTBRKT) {
                if ((updateCurrentToken()).token_type != TK_OF) {
                    token_error(currentTokenAddr, "Wrong array setting!28");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong array setting!29");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong array setting!30");
            //终止
        }
    } else {
        token_error(currentTokenAddr, "Wrong array setting!31");
        //终止
    }
    while ((updateCurrentToken()).token_type == TK_ARRAY) {
        if ((updateCurrentToken()).token_type == TK_LEFTBRKT) {
            if ((updateCurrentToken()).token_type == TK_NUM) {
                if ((updateCurrentToken()).token_type == TK_RIGHTBRKT) {
                    if ((updateCurrentToken()).token_type != TK_OF) {
                        token_error(currentTokenAddr, "Wrong array setting!32");
                        //终止
                    }
                } else {
                    token_error(currentTokenAddr, "Wrong array setting!33");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong array setting!34");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong array setting!35");
            //终止
        }
    }
    if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_CHARTYPE ||
        currentToken.token_type == TK_STRINGTYPE || currentToken.token_type == TK_BOOL) {
        if ((updateCurrentToken()).token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong array setting!36");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong array setting!37");
            //终止
        }
    } else if (currentToken.token_type == TK_STRUCT) {
        if ((updateCurrentToken()).token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong array setting!38");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong array setting!39");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong array setting!40");
            //终止
        }
    } else {
        token_error(currentTokenAddr, "Wrong array setting!41");
        //终止
    }
}

void StructSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_CHAR ||
            currentToken.token_type == TK_STR || currentToken.token_type == TK_IDENT ||
            currentToken.token_type == TK_TRUE || currentToken.token_type == TK_FALSE) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong struct setting!43");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong struct setting!43");
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {
                updateCurrentToken();
            } else {
                token_error(currentTokenAddr, "Wrong struct setting!42");
                //终止
            }
        }
        if (currentToken.token_type == TK_ASSIGN) {
            //判断是否属于值
            updateCurrentToken();
            if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_CHAR ||
                currentToken.token_type == TK_STR || currentToken.token_type == TK_IDENT ||
                currentToken.token_type == TK_TRUE || currentToken.token_type == TK_FALSE) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong struct setting!43");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong struct setting!43");
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            token_error(currentTokenAddr, "Wrong struct setting!110");
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong struct setting!44");
        //终止
    }
}

void PointSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong Point setting! 106");
                //终止
            }
        } else if (currentToken.token_type == TK_AND) {
            if ((updateCurrentToken()).token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong Point setting! 106");
                    //终止
                }
            } else {
                token_error(currentTokenAddr, "Wrong Point setting! 107");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong Point setting! 108");
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {
                updateCurrentToken();
            } else {
                token_error(currentTokenAddr, "Wrong Point setting!45");
                //终止
            }
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if ((updateCurrentToken()).token_type == TK_AND) {
                if ((updateCurrentToken()).token_type != TK_IDENT) {
                    token_error(currentTokenAddr, "Wrong Point setting!46");
                    //终止
                }
            } else if (currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    token_error(currentTokenAddr, "Wrong Point setting! 109");
                    //终止
                }
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            token_error(currentTokenAddr, "Wrong Point setting!48");
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong Point setting!109");
        //终止
    }
}

void addParameter(Declaration_node *func) {//有参数
    consume(TK_STRUCT);
    Type *ty = getType(currentToken);
    updateCurrentToken();
    if (ty == nullptr) {
        token_error(&currentToken, "Type not found");
    }
    while (consume(TK_MUL)) {
        ty = ptr_to(ty);
    }
    auto tmp = currentTokenAddr;
    expect(TK_IDENT);
    //声明局部变量一个
    func->parms.push_back(new Declaration_node(ND_VARDEF, tmp, ty));
}

void FunctionDecl(Type *returning, Token *tok, Nodebase *env) {
    Type *functype = new Type(FUNC, 0);
    functype->returning = returning;
    //放进类型表
    auto pos = types.find(tok->Name);
    if (pos != types.end()) {
        // 防止重复声明
        token_error(tok, "This function has been defined.");
    }
    types[tok->Name] = functype;

    auto func = new Declaration_node(ND_FUNC, tok, functype);
    if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_STRINGTYPE ||
        currentToken.token_type == TK_CHARTYPE || currentToken.token_type == TK_BOOL ||
        currentToken.token_type == TK_STRUCT) {//TODO array
        addParameter(func);
        while (consume(TK_COMMA)) {
            addParameter(func);
        }
    }
    //if (currentToken.token_type == TK_RIGHTPAR) {
    expect(TK_RIGHTPAR);
    if (consume(TK_LEFTBRACE)) {
        while (!consume(TK_RIGHTBRACE)) {
            auto st = ControlStream(func);
            if (st != nullptr) {
                func->stmts.push_back(st);
            }
        }
    } else/* (currentToken.token_type != TK_SEMICOLON)*/ {//TODO 先不考虑只有声明没有定义的情况
        expect(TK_SEMICOLON, "Wrong function!50");
        //终止
    }
    env->stmts.push_back(func);
}

Nodebase *ControlStream(Nodebase *env) {
    //声明
    if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_CHARTYPE ||
        currentToken.token_type == TK_STRINGTYPE || currentToken.token_type == TK_BOOL ||
        currentToken.token_type == TK_STRUCT || currentToken.token_type == TK_ARRAY) {
        Type *ty;
        tempToken = currentToken;//int处
        int a = ExceptStructOrArray();//第一个Token的类型 0:struct  1:基本类型 2:数组
        if (a == 2) { //数组类型
            ArraySetting();
        } else if (a == 1) {//基本类型
            ty = getType(tempToken);
            //指针判断应当放在这
            while (consume(TK_MUL)) {
                ty = ptr_to(ty);
            }
            auto tmp = currentTokenAddr;
            //if (consume(TK_IDENT)) {
            expect(TK_IDENT);
            if (consume(TK_LEFTPAR)) {
                FunctionDecl(ty, tmp, env);
            } else {
                variableDef(ty, tmp, env);
                while (consume(TK_COMMA)) {
                    tmp = currentTokenAddr;
                    if (consume(TK_IDENT)) {
                        variableDef(ty, tmp, env);
                    }
                }
                expect(TK_SEMICOLON);
            }
        } else {//struct开头//TODO
            ty = getType(currentToken);
            ty = ty ? ty : new Type(STRUCT, -1);
            //指针判断应当放在这
            expect(TK_IDENT);
            while (consume(TK_MUL)) {
                ty = ptr_to(ty);
            }
            auto tmp = currentTokenAddr;
            //if (consume(TK_IDENT)) {
            expect(TK_IDENT);
            if (consume(TK_LEFTPAR)) {
                FunctionDecl(ty, tmp, env);
            } else {
                StructSetting();
            }
            expect(TK_SEMICOLON);
        }
        return nullptr;
    } else {//表达式与语句
        Token *t = currentTokenAddr;
        switch (t->token_type) {
            case TK_IF: {//if
                updateCurrentToken();
                auto node = new Expression_Statement_node(ND_IF, t);
                expect(TK_LEFTPAR);
                node->cond = expr();
                expect(TK_RIGHTPAR);

                node->body = new Expression_Statement_node(ND_COMP_STMT, t);
                if (consume(TK_LEFTBRACE)) {
                    while (!consume(TK_RIGHTBRACE)) {
                        auto st = ControlStream(node->body);
                        if (st != nullptr) {
                            node->body->stmts.push_back(st);
                        }
                    }
                } else {
                    auto st = ControlStream(node->body);
                    if (st != nullptr) {
                        node->body->stmts.push_back(st);
                    }
                }

                if (consume(TK_ELSE)) {
                    node->else_body = new Expression_Statement_node(ND_COMP_STMT, t);
                    if (consume(TK_LEFTBRACE)) {
                        while (!consume(TK_RIGHTBRACE)) {
                            auto st = ControlStream(node->else_body);
                            if (st != nullptr) {
                                node->else_body->stmts.push_back(st);
                            }
                        }
                    } else {
                        auto st = ControlStream(node->else_body);
                        if (st != nullptr) {
                            node->else_body->stmts.push_back(st);
                        }
                    }

                }
                return node;
            }
            case TK_FOR: {
                updateCurrentToken();
                auto node = new Expression_Statement_node(ND_FOR, t);
                expect(TK_LEFTPAR);
                //env = new_env(env);
                //vec_push(breaks, node);
                //vec_push(continues, node);

                /*if (is_typename())
                    node->init = declaration();
                else */if (!consume(TK_SEMICOLON)) {
                    node->init = expr();
                    expect(TK_SEMICOLON);
                }

                if (!consume(TK_SEMICOLON)) {
                    node->cond = expr();
                    expect(TK_SEMICOLON);
                }

                if (!consume(TK_RIGHTPAR)) {
                    node->inc = expr();
                    expect(TK_RIGHTPAR);
                }

                node->body = new Expression_Statement_node(ND_COMP_STMT, t);
                if (consume(TK_LEFTBRACE)) {
                    while (!consume(TK_RIGHTBRACE)) {
                        auto st = ControlStream(node->body);
                        if (st != nullptr) {
                            node->body->stmts.push_back(st);
                        }
                    }
                } else {
                    auto st = ControlStream(node->body);
                    if (st != nullptr) {
                        node->body->stmts.push_back(st);
                    }
                }

                //vec_pop(breaks);
                //vec_pop(continues);
                //env = env->prev;
                return node;
            }
            case TK_WHILE: {
                updateCurrentToken();
                auto node = new Expression_Statement_node(ND_FOR, t);
                //vec_push(breaks, node);
                //vec_push(continues, node);

                expect(TK_LEFTPAR);
                node->cond = expr();
                expect(TK_RIGHTPAR);
                node->body = new Expression_Statement_node(ND_COMP_STMT, t);
                if (consume(TK_LEFTBRACE)) {
                    while (!consume(TK_RIGHTBRACE)) {
                        auto st = ControlStream(node->body);
                        if (st != nullptr) {
                            node->body->stmts.push_back(st);
                        }
                    }
                } else {
                    auto st = ControlStream(node->body);
                    if (st != nullptr) {
                        node->body->stmts.push_back(st);
                    }
                }

                //vec_pop(breaks);
                //vec_pop(continues);
                return node;

            }
                /*
             case TK_DO: {
        updateCurrentToken();
                 Node *node = new_node(ND_DO_WHILE, t);
                 vec_push(breaks, node);
                 vec_push(continues, node);

                 node->body = stmt();
                 expect(TK_WHILE);
                 expect('(');
                 node->cond = expr();
                 expect(')');
                 expect(';');

                 vec_pop(breaks);
                 vec_pop(continues);
                 return node;
             }
             case TK_SWITCH: {
        updateCurrentToken();
                 Node *node = new_node(ND_SWITCH, t);
                 node->cases = new_vec();

                 expect('(');
                 node->cond = expr();
                 expect(')');

                 vec_push(breaks, node);
                 vec_push(switches, node);
                 node->body = stmt();
                 vec_pop(breaks);
                 vec_pop(switches);
                 return node;
             }
             case TK_CASE: {
        updateCurrentToken();
                 if (switches->len == 0)
                     bad_token(t, "stray case");
                 Node *node = new_node(ND_CASE, t);
                 node->val = const_expr();
                 expect(':');
                 node->body = stmt();

                 Node *n = vec_last(switches);
                 vec_push(n->cases, node);
                 return node;
             }
             case TK_BREAK: {
        updateCurrentToken();
                 if (breaks->len == 0)
                     bad_token(t, "stray break");
                 Node *node = new_node(ND_BREAK, t);
                 node->target = vec_last(breaks);
                 return node;
             }
             case TK_CONTINUE: {
        updateCurrentToken();
                 if (continues->len == 0)
                     bad_token(t, "stray continue");
                 Node *node = new_node(ND_CONTINUE, t);
                 node->target = vec_last(breaks);
                 return node;
             }*/
            case TK_RETURN: {
                updateCurrentToken();
                auto node = new Expression_Statement_node(ND_RETURN, currentTokenAddr);
                consume(TK_RETURN);
                node->returnval = expr();
                expect(TK_SEMICOLON);
                return node;
            }
            case TK_LEFTBRACE: {
                updateCurrentToken();
                auto node = new Expression_Statement_node(ND_COMP_STMT, t);
                while (!consume(TK_RIGHTBRACE)) {
                    auto st = ControlStream(node->body);
                    if (st != nullptr) {
                        node->body->stmts.push_back(st);
                    }
                }
                return node;
            }
            case TK_SEMICOLON:
                consume(TK_SEMICOLON);
                return nullptr;
            default:
                return expr();
        }
    }
    /*

    updateCurrentToken();
    if (currentToken.token_type == TK_LEFTPAR) {
        IfParameter();
    }
    while (currentToken.token_type == TK_COMMA) {//赋值
        updateCurrentToken();
        if ((VariableOutcome = VariableName()) != 1) {
            token_error(currentTokenAddr, "Wrong function!63");
            //终止
        }
        updateCurrentToken();
    }
    if (currentToken.token_type == TK_ADD_EQ || currentToken.token_type == TK_SUB_EQ) {
        while (currentToken.token_type != TK_SEMICOLON) {
            Expression();
        }
    } else if (currentToken.token_type == TK_ASSIGN) {
        while (currentToken.token_type != TK_SEMICOLON) {
            Expression();
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong function!66");
        //终止
    }
    updateCurrentToken();
} else if (currentToken.token_type == TK_BREAK) {
    if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong function!68");
        //终止
    }
    updateCurrentToken();
} else if (currentToken.token_type == TK_CONTINUE) {
    if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong function!69");
        //终止
    }
    updateCurrentToken();
} else if (currentToken.token_type == TK_RETURN) {
    Expression();
    if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
        token_error(currentTokenAddr, "Wrong function!70");
        //终止
    }
    updateCurrentToken();
} else if (currentToken.token_type == TK_IF) {
    IfExpression();
} else if (currentToken.token_type == TK_WHILE) {
    WhileExpression();
    updateCurrentToken();
} else
    token_error(currentTokenAddr, "Wrong function!71");
//终止
     */
}

int ExceptStructOrArray() {
    if (consume(TK_ARRAY)) {
        return 2;
    } else if (consume(TK_NUMBERTYPE) || consume(TK_CHARTYPE) ||
               consume(TK_STRINGTYPE) || consume(TK_BOOL)) {
        return 1;
    } else {
        expect(TK_STRUCT);
        return 0;
    }
}


Nodebase *syntax() {
    auto *program = new Nodebase(ND_PROG, "Program start!");
    updateCurrentToken();
    while (currentToken.token_type != TK_EOF) {
        Program(program);
    }
    return program;
}
/*
void Expression() {
    updateCurrentToken();
}

void IfExpression() {
    expect(TK_LEFTPAR, "Has no leftpar");
    while (currentToken.token_type != TK_RIGHTPAR) {
        Expression();
    }
    expect(TK_RIGHTPAR, "Has no rightpar");
    expect(TK_LEFTBRACE, "Has no leftbrace");
    while (currentToken.token_type != TK_RIGHTBRACE) {
        ControlStream();
    }
    expect(TK_RIGHTBRACE, "Has no rightbrace");
    if (currentToken.token_type == TK_ELSE) {
        updateCurrentToken();
        if (currentToken.token_type == TK_IF) {
            updateCurrentToken();
            IfExpression();
        }
        expect(TK_LEFTBRACE, "Has no leftbrace");
        while (currentToken.token_type != TK_RIGHTBRACE) {
            ControlStream();
        }
        expect(TK_RIGHTBRACE, "Has no rightbrace");
    }
}


void WhileExpression() {
    expect(TK_LEFTPAR, "Has no leftpar");
    while (currentToken.token_type != TK_RIGHTPAR) {
        Expression();
    }
    expect(TK_RIGHTPAR, "Has no rightpar");
    expect(TK_LEFTBRACE, "Wrong While function!75");
    while (currentToken.token_type != TK_RIGHTBRACE) {
        //TODO ControlStream();
    }
}
*/

/*
int IfType() {
    if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_CHARTYPE ||
        currentToken.token_type == TK_STRINGTYPE || currentToken.token_type == TK_BOOL) {
        return 1;
    } else {
        token_error(currentTokenAddr, "Wrong FunctionDecl!79");
        //终止
    }
}

void IfParameter() {//函数是否有参数
    updateCurrentToken();
    if (currentToken.token_type == TK_RIGHTPAR) {//无参数
        if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
            token_error(currentTokenAddr, "Wrong function!80");
            //终止
        }
    } else if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_STRINGTYPE ||
               currentToken.token_type == TK_CHARTYPE || currentToken.token_type == TK_BOOL ||
               currentToken.token_type == TK_STRUCT) {
        addParameter(nullptr);
        while ((updateCurrentToken()).token_type == TK_COMMA) {
            updateCurrentToken();
            addParameter(nullptr);
        }
        if (currentToken.token_type == TK_RIGHTPAR) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                token_error(currentTokenAddr, "Wrong function!81");
                //终止
            }
        } else {
            token_error(currentTokenAddr, "Wrong function!82");
            //终止
        }
    } else {
        token_error(currentTokenAddr, "Wrong function!83");
        //终止
    }
}
 */

