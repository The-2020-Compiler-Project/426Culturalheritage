#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include "lex.h"
#include "Grammar.h"
#include "parse.h"
#include "../util/error_diagnostics.h"

using namespace std;
static list<Token> Token_list;

static auto *program = new Nodebase(ND_PROG, "Program start!");
static std::unordered_map<string, Type *> types{
        {{"void"},   new Type{VOID, 0}},
        {{"bool"},   new Type{BOOL, 1}},
        {{"char"},   new Type{CHAR, 1}},
        {{"number"}, new Type{NUMBER, 8}},
};
static Token currentToken;
#define currentTokenAddr &Token_list.back()
static int VariableOutcome;///////////////////加报错输出
static Token tempToken;

Type *getType(const Token &tok) {
    auto find = types.find(tok.Name);
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
        //TODO:bad
        token_error(currentTokenAddr, msg ? "Unexpected," : msg);
    }
}

Nodebase *syntax() {
    updateCurrentToken();
    while (currentToken.token_type != TK_EOF) {
        Program();
    }
    return program;
}

Type *ptr_to(Type *tp) {
    Type *ptr = new Type(PTR, 8, tp);
    return ptr;
}

/*
Expression_Statement_node *stmt_expr() {
    Token *t = tokens->data[pos];
    Vector *v = new_vec();

    env = new_env(env);
    do {
        vec_push(v, stmt());
    } while (!consume('}'));
    expect(')');
    env = env->prev;

    Node *last = vec_pop(v);
    if (last->op != ND_EXPR_STMT)
        bad_token(last->token, "statement expression returning void");

    Node *node = new_node(ND_STMT_EXPR, t);
    node->stmts = v;
    node->expr = last->expr;
    return node;
}

Expression_Statement_node *primary() {
    Token *t = tokens->data[pos++];

    if (t->ty == '(') {
        auto node = expr();
        expect(')');
        return node;
    }

    if (t->ty == TK_NUM)
        return new_int_node(t->val, t);

    if (t->ty == TK_STR)
        return string_literal(t);

    if (t->ty == TK_IDENT) {
        if (consume('('))
            return function_call(t);
        return local_variable(t);
    }

    bad_token(t, "primary expression expected");
}

Expression_Statement_node *mul();

// `x++` where x is of type T is compiled as
// `({ T *y = &x; T z = *y; *y = *y + 1; *z; })`.
Expression_Statement_node *new_post_inc(Token *t, Node *e, int imm) {
    Vector *v = new_vec();

    Var *var1 = add_lvar(ptr_to(e->ty), "tmp");
    Var *var2 = add_lvar(e->ty, "tmp");

    vec_push(v, new_binop('=', t, new_varref(t, var1), new_expr(ND_ADDR, t, e)));
    vec_push(v, new_binop('=', t, new_varref(t, var2), new_deref(t, var1)));
    vec_push(v, new_binop(
            '=', t, new_deref(t, var1),
            new_binop('+', t, new_deref(t, var1), new_int_node(imm, t))));
    vec_push(v, new_varref(t, var2));
    return new_stmt_expr(t, v);
}

Expression_Statement_node *postfix() {
    Node *lhs = primary();

    for (;;) {
        Token *t = tokens->data[pos];

        if (consume(TK_INC)) {
            lhs = new_post_inc(t, lhs, 1);
            continue;
        }

        if (consume(TK_DEC)) {
            lhs = new_post_inc(t, lhs, -1);
            continue;
        }

        if (consume('.')) {
            lhs = new_expr(ND_DOT, t, lhs);
            lhs->name = ident();
            continue;
        }

        if (consume(TK_ARROW)) {
            lhs = new_expr(ND_DOT, t, new_expr(ND_DEREF, t, lhs));
            lhs->name = ident();
            continue;
        }

        if (consume('[')) {
            Node *node = new_binop('+', t, lhs, assign());
            lhs = new_expr(ND_DEREF, t, node);
            expect(']');
            continue;
        }
        return lhs;
    }
}

Expression_Statement_node *new_assign_eq(int op, Node *lhs, Node *rhs);

void variableDef(Type *t, Token *ptr);

Expression_Statement_node *unary() {
    Token *t = tokens->data[pos];

    if (consume('-'))
        return new_binop('-', t, new_int_node(0, t), unary());
    if (consume('*'))
        return new_expr(ND_DEREF, t, unary());
    if (consume('&'))
        return new_expr(ND_ADDR, t, unary());
    if (consume('!'))
        return new_expr('!', t, unary());
    if (consume('~'))
        return new_expr('~', t, unary());
    if (consume(TK_SIZEOF))
        return new_int_node(get_type(unary())->size, t);
    if (consume(TK_ALIGNOF))
        return new_int_node(get_type(unary())->align, t);
    if (consume(TK_INC))
        return new_assign_eq('+', unary(), new_int_node(1, t));
    if (consume(TK_DEC))
        return new_assign_eq('-', unary(), new_int_node(1, t));
    return postfix();
}

Expression_Statement_node *mul() {
    Node *lhs = unary();
    for (;;) {
        Token *t = tokens->data[pos];
        if (consume('*'))
            lhs = new_binop('*', t, lhs, unary());
        else if (consume('/'))
            lhs = new_binop('/', t, lhs, unary());
        else if (consume('%'))
            lhs = new_binop('%', t, lhs, unary());
        else
            return lhs;
    }
}

Expression_Statement_node *add() {
    Node *lhs = mul();
    for (;;) {
        Token *t = tokens->data[pos];
        if (consume('+'))
            lhs = new_binop('+', t, lhs, mul());
        else if (consume('-'))
            lhs = new_binop('-', t, lhs, mul());
        else
            return lhs;
    }
}

Expression_Statement_node *shift() {
    Node *lhs = add();
    for (;;) {
        Token *t = tokens->data[pos];
        if (consume(TK_SHL))
            lhs = new_binop(ND_SHL, t, lhs, add());
        else if (consume(TK_SHR))
            lhs = new_binop(ND_SHR, t, lhs, add());
        else
            return lhs;
    }
}

Expression_Statement_node *relational() {
    Node *lhs = shift();
    for (;;) {
        Token *t = tokens->data[pos];
        if (consume('<'))
            lhs = new_binop('<', t, lhs, shift());
        else if (consume('>'))
            lhs = new_binop('<', t, shift(), lhs);
        else if (consume(TK_LE))
            lhs = new_binop(ND_LE, t, lhs, shift());
        else if (consume(TK_GE))
            lhs = new_binop(ND_LE, t, shift(), lhs);
        else
            return lhs;
    }
}

Expression_Statement_node *equality() {
    Node *lhs = relational();
    for (;;) {
        Token *t = tokens->data[pos];
        if (consume(TK_EQ))
            lhs = new_binop(ND_EQ, t, lhs, relational());
        else if (consume(TK_NE))
            lhs = new_binop(ND_NE, t, lhs, relational());
        else
            return lhs;
    }
}

Expression_Statement_node *bit_and() {
    Node *lhs = equality();
    while (consume('&')) {
        Token *t = tokens->data[pos];
        lhs = new_binop('&', t, lhs, equality());
    }
    return lhs;
}

Expression_Statement_node *bit_xor() {
    Node *lhs = bit_and();
    while (consume('^')) {
        Token *t = tokens->data[pos];
        lhs = new_binop('^', t, lhs, bit_and());
    }
    return lhs;
}

Expression_Statement_node *bit_or() {
    Node *lhs = bit_xor();
    while (consume('|')) {
        Token *t = tokens->data[pos];
        lhs = new_binop('|', t, lhs, bit_xor());
    }
    return lhs;
}

Expression_Statement_node *logand() {
    Node *lhs = bit_or();
    while (consume(TK_LOGAND)) {
        Token *t = tokens->data[pos];
        lhs = new_binop(ND_LOGAND, t, lhs, bit_or());
    }
    return lhs;
}

Expression_Statement_node *logor() {
    Node *lhs = logand();
    while (consume(TK_LOGOR)) {
        Token *t = tokens->data[pos];
        lhs = new_binop(ND_LOGOR, t, lhs, logand());
    }
    return lhs;
}

Expression_Statement_node *conditional() {
    Node *cond = logor();
    Token *t = tokens->data[pos];
    if (!consume('?'))
        return cond;

    Node *node = new_node('?', t);
    node->cond = cond;
    node->then = expr();
    expect(':');
    node->els = conditional();
    return node;
}

// `x op= y` where x is of type T is compiled as
// `({ T *z = &x; *z = *z op y; })`.
Expression_Statement_node *new_assign_eq(int op, Node *lhs, Node *rhs) {
    Vector *v = new_vec();
    Token *t = lhs->token;

    // T *z = &x
    Var *var = add_lvar(ptr_to(lhs->ty), "tmp");
    vec_push(v, new_binop('=', t, new_varref(t, var), new_expr(ND_ADDR, t, lhs)));

    // *z = *z op y
    vec_push(v, new_binop('=', t, new_deref(t, var),
                          new_binop(op, t, new_deref(t, var), rhs)));
    return new_stmt_expr(t, v);
}

Expression_Statement_node *assign() {
    Node *lhs = conditional();
    Token *t = tokens->data[pos];

    if (consume('='))
        return new_binop('=', t, lhs, assign());
    if (consume(TK_MUL_EQ))
        return new_assign_eq('*', lhs, assign());
    if (consume(TK_DIV_EQ))
        return new_assign_eq('/', lhs, assign());
    if (consume(TK_MOD_EQ))
        return new_assign_eq('%', lhs, assign());
    if (consume(TK_ADD_EQ))
        return new_assign_eq('+', lhs, assign());
    if (consume(TK_SUB_EQ))
        return new_assign_eq('-', lhs, assign());
    if (consume(TK_SHL_EQ))
        return new_assign_eq(ND_SHL, lhs, assign());
    if (consume(TK_SHR_EQ))
        return new_assign_eq(ND_SHR, lhs, assign());
    if (consume(TK_AND_EQ))
        return new_assign_eq(ND_LOGAND, lhs, assign());
    if (consume(TK_XOR_EQ))
        return new_assign_eq('^', lhs, assign());
    if (consume(TK_OR_EQ))
        return new_assign_eq('|', lhs, assign());
    return lhs;
}
 */

Expression_Statement_node *expr() {
    /*
    Node *lhs = assign();
    Token *t = tokens->data[pos];
    if (!consume(','))
        return lhs;
    return new_binop(',', t, lhs, expr());
     */
    return new Expression_Statement_node(ND_NUM, currentTokenAddr);
}

Nodebase *variableInit(Type *t, Token *var) {
    auto assignToken = &*(++Token_list.rbegin());
    auto _equ =
            new Expression_Statement_node(ND_ASSIGN, assignToken);

    _equ->lhs = new Expression_Statement_node(ND_VARREF, var);

    _equ->rhs = expr();//表达式
    updateCurrentToken();
    return _equ;
}

void variableDef(Type *t, Token *var, Nodebase *root) {
    root->stmts.push_back(new Declaration_node(ND_VARDEF, var, t));
    if (consume(TK_ASSIGN)) {
        root->stmts.push_back(variableInit(t, var));
    }
}

void Program() {
    //类型
    //标识符
    Type *ty;
    tempToken = currentToken;//int处
    int a = ExceptStructOrArray();//第一个Token的类型 1:基本类型 2:数组
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
            FunctionDecl(ty, tmp);
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
                cout << "Wrong type!" << endl;
                //终止
            }
        }
        else {
            cout << "Wrong type!1" << endl;
            //终止
        }
         */

    } else if (a == 0) {//struct开头//TODO
        if (consume(TK_IDENT)) {
            updateCurrentToken();
            if (currentToken.token_type == TK_LEFTBRACE) {
                //为{，开始struct声明
                StructNaming();
            } else if (currentToken.token_type == TK_IDENT) {
                updateCurrentToken();
                if (currentToken.token_type == TK_LEFTPAR) {
                    FunctionDecl(nullptr, nullptr);
                } else {
                    StructSetting();
                }
            } else {
                cout << "Wrong StructSetting!2" << endl;
                //终止
            }
        } else if (currentToken.token_type == TK_MUL) {
            while ((updateCurrentToken()).token_type == TK_MUL) {
                continue;
            }
            if (currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
                    FunctionDecl(nullptr, nullptr);
                } else {
                    PointSetting();
                }
            }
        } else {
            cout << "Wrong StructSetting!3" << endl;
        }
    } else {
        cout << "Wrong StructSetting!4" << endl;
    }
}


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
        cout << "Wrong VariableSetting!5" << endl;
        //终止
    }
}

int VariableName() {
    if (currentToken.token_type == TK_IDENT) {
        return 1;
    } else {
        //终止
        cout << "Wrong VariableName!6" << endl;

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
                cout << "Wrong type!7" << endl;
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
                        cout << "Wrong StructNaming!8" << endl;
                        //终止
                    }
                } else {
                    cout << "Wrong StructNaming!9" << endl;
                    //终止
                }
            } else {
                cout << "Wrong StructNaming!10" << endl;
                //终止
            }
        }
    }
    if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
        cout << "Wrong StructNaming!11" << endl;
        //终止
    }
}

void NumberSetting() {//赋值
    if (currentToken.token_type == TK_ADD_EQ || currentToken.token_type == TK_SUB_EQ) {
        updateCurrentToken();
        //TODO:贾浩楠,怎么用表达式赋值
        if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                cout << "Wrong number setting111!" << endl;
                //终止
            }
        } else {
            cout << "Wrong number setting112!" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                cout << "Wrong number setting101!" << endl;
                //终止
            }
        } else {
            cout << "Wrong number setting121!" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong number setting!12" << endl;
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong number setting!13" << endl;
                    //终止
                }
            } else {
                cout << "Wrong number setting!14" << endl;
                //终止
            }
        } else if (currentToken.token_type == TK_SUB_EQ || currentToken.token_type == TK_ADD_EQ) {
            updateCurrentToken();
            if (currentToken.token_type == TK_NUM || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong number setting111!" << endl;
                    //终止
                }
            } else {
                cout << "Wrong number setting112!" << endl;
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            cout << "Wrong number setting!15" << endl;
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        cout << "Wrong number setting!103" << endl;
        //终止
    }
}

void StringSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_STR || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                cout << "Wrong String setting101!" << endl;
                //终止
            }
        } else {
            cout << "Wrong String setting101!" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong string setting!16" << endl;
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_STR || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong string setting!17" << endl;
                    //终止
                }
            } else {
                cout << "Wrong string setting!18" << endl;
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            cout << "Wrong string setting!113" << endl;
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        cout << "Wrong string setting!19" << endl;
        //终止
    }
}

void CharSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_CHAR || currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                cout << "Wrong char setting102!" << endl;
                //终止
            }
        } else {
            cout << "Wrong char setting103!" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong char setting!20" << endl;
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_CHAR || currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong char setting!21" << endl;
                    //终止
                }
            } else {
                cout << "Wrong char setting!22" << endl;
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            cout << "Wrong char setting!112" << endl;
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        cout << "Wrong char setting!23" << endl;
        //终止
    }
}

void BoolSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_TRUE || currentToken.token_type == TK_FALSE ||
            currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                cout << "Wrong bool setting101!" << endl;
                //终止
            }
        } else {
            cout << "Wrong bool setting101!" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong bool setting!24" << endl;
                //终止
            }
            updateCurrentToken();
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if (currentToken.token_type == TK_TRUE || currentToken.token_type == TK_FALSE ||
                currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong bool setting!25" << endl;
                    //终止
                }
            } else {
                cout << "Wrong bool setting!26" << endl;
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            cout << "Wrong bool setting!111" << endl;
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        cout << "Wrong bool setting!27" << endl;
        //终止
    }
}

void ArraySetting() {
    if ((updateCurrentToken()).token_type == TK_LEFTBRKT) {
        if ((updateCurrentToken()).token_type == TK_NUM) {
            if ((updateCurrentToken()).token_type == TK_RIGHTBRKT) {
                if ((updateCurrentToken()).token_type != TK_OF) {
                    cout << "Wrong array setting!28" << endl;
                    //终止
                }
            } else {
                cout << "Wrong array setting!29" << endl;
                //终止
            }
        } else {
            cout << "Wrong array setting!30" << endl;
            //终止
        }
    } else {
        cout << "Wrong array setting!31" << endl;
        //终止
    }
    while ((updateCurrentToken()).token_type == TK_ARRAY) {
        if ((updateCurrentToken()).token_type == TK_LEFTBRKT) {
            if ((updateCurrentToken()).token_type == TK_NUM) {
                if ((updateCurrentToken()).token_type == TK_RIGHTBRKT) {
                    if ((updateCurrentToken()).token_type != TK_OF) {
                        cout << "Wrong array setting!32" << endl;
                        //终止
                    }
                } else {
                    cout << "Wrong array setting!33" << endl;
                    //终止
                }
            } else {
                cout << "Wrong array setting!34" << endl;
                //终止
            }
        } else {
            cout << "Wrong array setting!35" << endl;
            //终止
        }
    }
    if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_CHARTYPE ||
        currentToken.token_type == TK_STRINGTYPE || currentToken.token_type == TK_BOOL) {
        if ((updateCurrentToken()).token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                cout << "Wrong array setting!36" << endl;
                //终止
            }
        } else {
            cout << "Wrong array setting!37" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_STRUCT) {
        if ((updateCurrentToken()).token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong array setting!38" << endl;
                    //终止
                }
            } else {
                cout << "Wrong array setting!39" << endl;
                //终止
            }
        } else {
            cout << "Wrong array setting!40" << endl;
            //终止
        }
    } else {
        cout << "Wrong array setting!41" << endl;
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
                cout << "Wrong struct setting!43" << endl;
                //终止
            }
        } else {
            cout << "Wrong struct setting!43" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {
                updateCurrentToken();
            } else {
                cout << "Wrong struct setting!42" << endl;
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
                    cout << "Wrong struct setting!43" << endl;
                    //终止
                }
            } else {
                cout << "Wrong struct setting!43" << endl;
                //终止
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            cout << "Wrong struct setting!110" << endl;
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        cout << "Wrong struct setting!44" << endl;
        //终止
    }
}

void PointSetting() {
    if (currentToken.token_type == TK_ASSIGN) {
        updateCurrentToken();
        if (currentToken.token_type == TK_IDENT) {
            if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                cout << "Wrong Point setting! 106" << endl;
                //终止
            }
        } else if (currentToken.token_type == TK_AND) {
            if ((updateCurrentToken()).token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong Point setting! 106" << endl;
                    //终止
                }
            } else {
                cout << "Wrong Point setting! 107" << endl;
                //终止
            }
        } else {
            cout << "Wrong Point setting! 108" << endl;
            //终止
        }
    } else if (currentToken.token_type == TK_COMMA) {
        while (currentToken.token_type == TK_COMMA) {
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {
                updateCurrentToken();
            } else {
                cout << "Wrong Point setting!45" << endl;
                //终止
            }
        }
        if (currentToken.token_type == TK_ASSIGN) {
            updateCurrentToken();
            if ((updateCurrentToken()).token_type == TK_AND) {
                if ((updateCurrentToken()).token_type != TK_IDENT) {
                    cout << "Wrong Point setting!46" << endl;
                    //终止
                }
            } else if (currentToken.token_type == TK_IDENT) {
                if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
                    cout << "Wrong Point setting! 109" << endl;
                    //终止
                }
            }
        } else if (currentToken.token_type != TK_SEMICOLON) {
            cout << "Wrong Point setting!48" << endl;
            //终止
        }
    } else if (currentToken.token_type != TK_SEMICOLON) {
        cout << "Wrong Point setting!109" << endl;
        //终止
    }
}

void FunctionDecl(Type *returning, Token *tok) {
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
        while (currentToken.token_type != TK_RIGHTBRACE) {
            ControlStream();
        }
        expect(TK_RIGHTBRACE);
    } else/* (currentToken.token_type != TK_SEMICOLON)*/ {//TODO 先不考虑只有声明没有定义的情况
        expect(TK_SEMICOLON, "Wrong function!50");
        //终止
    }
}

void ControlStream() {
    if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_CHARTYPE ||
        currentToken.token_type == TK_STRINGTYPE || currentToken.token_type == TK_BOOL ||
        currentToken.token_type == TK_STRUCT) {
        int a = ExceptStructOrArray();
        tempToken = currentToken;
        if (a == 2) {
            ArraySetting();
        } else if (a == 1) {
            //别的类型
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {
                updateCurrentToken();
                VariableSetting();
            } else if (currentToken.token_type == TK_MUL) {
                while ((updateCurrentToken()).token_type == TK_MUL) {
                    continue;
                }
                if (currentToken.token_type == TK_IDENT) {
                    updateCurrentToken();
                    PointSetting();
                }
            }
        } else if (a == 0) {//struct开头
            updateCurrentToken();
            if (currentToken.token_type == TK_IDENT) {//结构体名
                updateCurrentToken();
                if (currentToken.token_type == TK_IDENT) {
                    updateCurrentToken();
                    StructSetting();
                } else if (currentToken.token_type == TK_MUL) {
                    while ((updateCurrentToken()).token_type == TK_MUL) {
                        continue;
                    }
                    if (currentToken.token_type == TK_IDENT) {
                        updateCurrentToken();
                        PointSetting();
                    } else {
                        cout << "Wrong ControlStream!53" << endl;
                        //终止
                    }
                } else
                    cout << "Wrong ControlStream!54" << endl;
                //终止
            } else
                cout << "Wrong ControlStream!55" << endl;
            //终止
        }
        updateCurrentToken();
    } else if (currentToken.token_type == TK_IDENT) {
        updateCurrentToken();
        if (currentToken.token_type == TK_LEFTPAR) {
            IfParameter();
        }
        while (currentToken.token_type == TK_COMMA) {//赋值
            updateCurrentToken();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong function!63" << endl;
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
            cout << "Wrong function!66" << endl;
            //终止
        }
        updateCurrentToken();
    } else if (currentToken.token_type == TK_BREAK) {
        if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
            cout << "Wrong function!68" << endl;
            //终止
        }
        updateCurrentToken();
    } else if (currentToken.token_type == TK_CONTINUE) {
        if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
            cout << "Wrong function!69" << endl;
            //终止
        }
        updateCurrentToken();
    } else if (currentToken.token_type == TK_RETURN) {
        Expression();
        if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
            cout << "Wrong function!70" << endl;
            //终止
        }
        updateCurrentToken();
    } else if (currentToken.token_type == TK_IF) {
        IfExpression();
    } else if (currentToken.token_type == TK_WHILE) {
        WhileExpression();
        updateCurrentToken();
    } else
        cout << "Wrong function!71" << endl;
    //终止
}

void Expression() {
    updateCurrentToken();
}

void IfExpression() {
    if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
        while (currentToken.token_type != TK_RIGHTPAR) {
            Expression();
        }
        if ((updateCurrentToken()).token_type == TK_LEFTBRACE) {
            updateCurrentToken();
            while (currentToken.token_type != TK_RIGHTBRACE) {
                ControlStream();
            }
            if ((updateCurrentToken()).token_type == TK_ELSE) {
                updateCurrentToken();
                if (currentToken.token_type == TK_IF) {
                    IfExpression();
                } else if (currentToken.token_type == TK_LEFTBRACE) {
                    updateCurrentToken();
                    while (currentToken.token_type != TK_RIGHTBRACE) {
                        ControlStream();
                    }
                    updateCurrentToken();
                } else {
                    cout << "Wrong If function!100" << endl;
                    //终止
                }
            }
        } else {
            cout << "Wrong If function!74" << endl;
            //终止
        }
    } else {
        cout << "Wrong If function!74" << endl;
        //终止
    }
}

void WhileExpression() {
    if ((updateCurrentToken()).token_type == TK_LEFTPAR) {
        while (currentToken.token_type != TK_RIGHTPAR) {
            Expression();
        }
        if ((updateCurrentToken()).token_type == TK_LEFTBRACE) {
            updateCurrentToken();
            while (currentToken.token_type != TK_RIGHTBRACE) {
                ControlStream();
            }
        } else {
            cout << "Wrong While function!75" << endl;
            //终止
        }
    } else {
        cout << "Wrong While function!77" << endl;
        //终止
    }
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

int IfType() {
    if (currentToken.token_type == TK_NUMBERTYPE || currentToken.token_type == TK_CHARTYPE ||
        currentToken.token_type == TK_STRINGTYPE || currentToken.token_type == TK_BOOL) {
        return 1;
    } else {
        cout << "Wrong FunctionDecl!79" << endl;
        //终止
    }
}

void IfParameter() {//函数是否有参数
    updateCurrentToken();
    if (currentToken.token_type == TK_RIGHTPAR) {//无参数
        if ((updateCurrentToken()).token_type != TK_SEMICOLON) {
            cout << "Wrong function!80" << endl;
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
                cout << "Wrong function!81" << endl;
                //终止
            }
        } else {
            cout << "Wrong function!82" << endl;
            //终止
        }
    } else {
        cout << "Wrong function!83" << endl;
        //终止
    }
}

void addParameter(Declaration_node *func) {//有参数
    consume(TK_STRUCT);
    Type *ty = getType(currentToken);
    auto tmp = currentTokenAddr;
    if (ty == nullptr) {
        token_error(tmp, "Type not found");
    }
    while (consume(TK_MUL)) {
        ty = ptr_to(ty);
    }
    tmp = currentTokenAddr;
    expect(TK_IDENT);
    //声明局部变量一个
    func->parms.push_back(new Declaration_node(ND_VARDEF, tmp, ty));

    /*
} else {
    cout << "Wrong function!86" << endl;
    //终止
}
} else if ((VariableOutcome = IfType()) == 1) { //TODO 啥意思？
    updateCurrentToken();
    if (currentToken.token_type == TK_MUL) {
        while ((updateCurrentToken()).token_type == TK_MUL) {
            continue;
        }
        if (currentToken.token_type != TK_IDENT) {
            cout << "Wrong function!87" << endl;
            //终止
        }
    } else if (currentToken.token_type != TK_IDENT) {
        cout << "Wrong function!88" << endl;
        //终止
    }
}
     */
}
/*

#define nowTokAddr &Token_list.back()

Type *getType(const Token &tok);


Type *getType(const Token &tok) {
    auto find = types.find(tok.Name);
    return (find==types.end()?nullptr:(*find).second);
}
Type *getType(const char *name) {
    auto find = types.find(name);
    return (find==types.end()?nullptr:(*find).second);
}

bool is_basic_types(const Token &tok){
    return tok.token_type == TK_NUMBERTYPE
    ||tok.token_type == TK_STRINGTYPE
    ||tok.token_type == TK_BOOL
    ||tok.token_type == TK_NUMBERTYPE
    ||tok.token_type == TK_VOID;
}


void Program(Token tok){
    Token_list.push_back(tok);
    Nodebase * res;

    if(is_basic_types(tok)){ //一定是变量声明
        auto d_n = new Declaration_node(ND_VARDEF, nowTokAddr, getType(tok));

        Token_list.push_back(get_token());
        if(Token_list.back().token_type == TK_IDENT){
            Token_list.push_back(get_token());
            if(Token_list.back().token_type == TK_MAIN){

            }else{
                Token_list.push_back(get_token());
                if(Token_list.back().token_type == TK_LEFTPAR){
                    //函数
                    functionDecl();
                }else{
                    //变量初始化
                    varsInit();
                }
            }
        }else{
            goto prog_err;
        }

    }else if(tok.token_type == TK_ARRAY) {//数组声明

    }else if(tok.token_type == TK_STRUCT ){//可能是变量声明或者是自定义类型声明
        Token_list.push_back(get_token());
        Token_list.push_back(get_token());
        Token_list.push_back(get_token());
        if((*Token_list.rbegin()).token_type==TK_LEFTBRACE){
            struct
        }

    }else{
        //报错
       prog_err:
    }
    return res;

}

*/
