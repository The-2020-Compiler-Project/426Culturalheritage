// parse.h向前类声明出错

#include "semantic.h"
#include "parse.h"

using namespace std;
static auto CT = new constTable();
static auto IT = new idenTable();
static int temp_count = 0;

/*
// double转char*
string doubleToStr(double n) {
    return to_string(n);
}

// 字符串(name)转double
double strToDouble(const char *s) {
    double ret = stod(string(s));
    return ret;
    double afterDot = 0.1;
    for(int i=0; i<strlen(s) ; i++){
        if( s[i] != '.' ){
            // 整数部分
            ret = ret*10 + s[i] - '0';
        } else {
            // 小数部分
            for(int j=i+1; j<strlen(s); j++){
                ret += (s[j] -'0') * afterDot;
                afterDot *= afterDot;
            }
            break;
        }
    }
}
*/
// 遍历树 获取一个结点的数值同时更新四元式
// 未创建临时变量
/*
double getNumber( Expression_Statement_node *p ){
    if( p->nodetype == ND_NUM ){
        // 如果是数值
        return strToDouble( p->getName() );
    } else if( p->nodetype == ND_VARREF ) {
        // 变量
        return IT->getValue(const_cast<char *>(p->getName()));
    } else {
        // 算术符号
        if( p->nodetype == ND_MUL ) {
            // 乘号
            return getNumber( p->lhs ) * getNumber(p->rhs);
        } else if( p->nodetype == ND_DIV ) {
            // 除号
            return getNumber( p->lhs ) / getNumber(p->rhs);
        } else if( p->nodetype == ND_ADD ) {
            // 加号
            return getNumber( p->lhs ) + getNumber(p->rhs);
        } else if( p->nodetype == ND_SUB ) {
            // 减号
            return getNumber( p->lhs ) - getNumber(p->rhs);
        }else if( p->nodetype == ND_MOD){
            return long(getNumber( p->lhs )) % long(getNumber(p->rhs));
        }

    }
}
 */

// 赋值语句 当前p结点为_equ
void Assignment(Quadruple *quadruple, Expression_Statement_node *p) {
    Expression_Statement_node *lhs = p->lhs;
    Expression_Statement_node *rhs = p->rhs;
    string value;
    if (IT->exist(lhs->getName())) {
        value = calculate(quadruple, rhs);
    } else {
        std::cout << "Identifier not found in IT" << std::endl;
        return;
    }
    // 生成四元式
    quadruple->addQuadruple("=", value, "_", lhs->getName());
    // 标识符表赋值
    IT->idTable[const_cast<char *>(lhs->getName())] = value;
}

// 递归 函数定义
void FuncDeclare(Quadruple *quadruple, Declaration_node *p) {
    quadruple->addQuadruple("func_de", "", "", p->getName());
    traverse(quadruple, p);
}

// Return语句
void RT(Quadruple *quadruple, Expression_Statement_node *p) {
    auto retValue = calculate(quadruple, dynamic_cast<Expression_Statement_node *>(p->returnval));
    quadruple->addQuadruple("return", "_", "_", retValue);
}

void IF(Quadruple *quadruple, Expression_Statement_node *p) {
    // IF语句
    auto *cond = dynamic_cast<Expression_Statement_node *>(p->cond);
    if (cond->nodetype == ND_EX) {
        string temp = "t";
        temp += to_string(temp_count++);
        IT->idTable[temp] = string("0");
        auto ex = dynamic_cast<Expression_Statement_node *>(cond->returnval);
        string s = calculate(quadruple, ex);
        quadruple->addQuadruple("!", s, "_", temp);

        traverse(quadruple, p->body);
        quadruple->addQuadruple("el", "_", "_", "_");
        traverse(quadruple, p->else_body);
        quadruple->addQuadruple("ie", "_", "_", "_");
        return;
    }

    auto *lhs = dynamic_cast<Expression_Statement_node *>(cond->lhs);
    auto *rhs = dynamic_cast<Expression_Statement_node *>(cond->rhs);
//    string temp = "t";
//    temp += to_string(temp_count++);
//    IT->append(temp);
    string leftStr = calculate(quadruple, lhs);
    IT->idTable[leftStr] = string("0");
    string rightStr = calculate(quadruple, rhs);
    IT->idTable[rightStr] = string("0");
    string temp = "t";
    temp += to_string(temp_count++);
    IT->idTable[temp] = string("0");
    if (cond->nodetype == ND_EQ) {
        // 等于
        quadruple->addQuadruple("==", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LT) {
        //小于
        quadruple->addQuadruple("<", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_GT) {
        //大于
        quadruple->addQuadruple(">", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LE) {
        //小于等于
        quadruple->addQuadruple("<=>", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_GE) {
        //大于等于
        quadruple->addQuadruple(">=", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LOGAND) {
        // AND
        quadruple->addQuadruple("&&", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LOGOR) {
        // OR
        quadruple->addQuadruple("||", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_NE) {
        // not equal
        quadruple->addQuadruple("!=", leftStr, rightStr, temp);
    } else {
        return;
    }
    quadruple->addQuadruple("if", temp, "_", "_");
    traverse(quadruple, p->body);
    quadruple->addQuadruple("el", "_", "_", "_");
    traverse(quadruple, p->else_body);
    quadruple->addQuadruple("ie", "_", "_", "_");
}


void FOR(Quadruple *quadruple, Expression_Statement_node *p) {
    // WH语句
    quadruple->addQuadruple("wh", "_", "_", "_");
    auto *cond = dynamic_cast<Expression_Statement_node *>(p->cond);
    if (cond->nodetype == ND_EX) {
        string temp = "t";
        temp += to_string(temp_count++);
        IT->idTable[temp] = string("0");
        auto ex = dynamic_cast<Expression_Statement_node *>(cond->returnval);
        string s = calculate(quadruple, ex);
        quadruple->addQuadruple("!", s, "_", temp);

        quadruple->addQuadruple("do", temp, "_", "_");
        traverse(quadruple, p->body);
        quadruple->addQuadruple("we", "_", "_", "_");
        return;
    }

//    string temp = "t";
//    temp += to_string(temp_count++);
//    IT->append(temp);
    auto *lhs = dynamic_cast<Expression_Statement_node *>(cond->lhs);
    auto *rhs = dynamic_cast<Expression_Statement_node *>(cond->rhs);
    string leftStr = calculate(quadruple, lhs);
    IT->idTable[leftStr] = string("0");
    string rightStr = calculate(quadruple, rhs);
    IT->idTable[rightStr] = string("0");
    string temp = "t";
    temp += to_string(temp_count++);
    IT->idTable[temp];
    if (cond->nodetype == ND_EQ) {
        // 等于
        quadruple->addQuadruple("==", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LT) {
        //小于
        quadruple->addQuadruple("<", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_GT) {
        //大于
        quadruple->addQuadruple(">", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LE) {
        //小于等于
        quadruple->addQuadruple("<=>", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_GE) {
        //大于等于
        quadruple->addQuadruple(">=", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LOGAND) {
        // AND
        quadruple->addQuadruple("&&", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_LOGOR) {
        // OR
        quadruple->addQuadruple("||", leftStr, rightStr, temp);
    } else if (cond->nodetype == ND_NE) {
        // not equal
        quadruple->addQuadruple("!=", leftStr, rightStr, temp);
    } else {
        return;
    }
    quadruple->addQuadruple("do", temp, "_", "_");
    traverse(quadruple, p->body);
    quadruple->addQuadruple("we", "_", "_", "_");

}

//遍历树更新四元式
void traverse(Quadruple *quadruple, Nodebase *p) {
    for (Nodebase *cur:p->stmts) {
        if (cur->nodetype == ND_VARDEF || cur->nodetype == ND_STRUCT) {
            // 声明结点
            if (IT->exist(cur->getName())) {
                token_error(cur->tok, "Identifier has been defined.");
            }
            IT->append(cur->getName());
        } else if (cur->nodetype == ND_ASSIGN) {
            // 赋值结点
            Expression_Statement_node *statementNode = dynamic_cast<Expression_Statement_node *>(cur);
            Assignment(quadruple, statementNode);
        } else if (cur->nodetype == ND_FUNC) {
            // 函数定义
            Declaration_node *declarationNode = dynamic_cast<Declaration_node *>(cur);
            FuncDeclare(quadruple, declarationNode);
            quadruple->addQuadruple("func_end", "", "", cur->getName());
        } else if (cur->nodetype == ND_RETURN) {
            // 函数return
            Expression_Statement_node *statementNode = dynamic_cast<Expression_Statement_node *>(cur);
            if (!statementNode->returnval) {
                // 函数没有返回值
                quadruple->addQuadruple("return", "", "", "void");
            } else {
                // 函数有返回值
                RT(quadruple, statementNode);
            }
        } else if (cur->nodetype == ND_IF) {
            // IF
            Expression_Statement_node *statementNode = dynamic_cast<Expression_Statement_node *>(cur);
            statementNode = dynamic_cast<Expression_Statement_node *>(cur);
            IF(quadruple, statementNode);
        } else if (cur->nodetype == ND_FOR) {
            Expression_Statement_node *statementNode = dynamic_cast<Expression_Statement_node *>(cur);
            statementNode = dynamic_cast<Expression_Statement_node *>(cur);
            FOR(quadruple, statementNode);
        }
    }
}

Quadruple *treeToQuad(Nodebase *p) {
    auto quadruple = new Quadruple();

    if (p->nodetype != ND_PROG) {
        cout << "Root Node is not Program!";
        return nullptr;
    } else {
        // 遍历program树生成四元式
        traverse(quadruple, p);
    }
    return quadruple;
}

void idenTable::print(ostream &out) {
    for (auto[x, y]:idTable) {
        out << x << ' ' << y << endl;
    }
}

idenTable *getIdenTable() {
    return IT;
};

string calculate(Quadruple *quadruple, Nodebase *p) {
    auto expressionNode = dynamic_cast<Expression_Statement_node *>(p);
    if (!expressionNode->lhs) {
        return expressionNode->getName();
    }
    string op;
    switch (expressionNode->nodetype) {
        case ND_ADD:
            op = '+';
            break;
        case ND_SUB:
            op = "-";
            break;
        case ND_MUL:
            op = '*';
            break;
        case ND_DIV:
            op = "/";
            break;
        case ND_MOD:
            op = '%';
            break;
        default:
            break;
    }
    auto lhs = dynamic_cast<Expression_Statement_node *>(expressionNode->lhs);
    auto rhs = dynamic_cast<Expression_Statement_node *>(expressionNode->rhs);
    string temp = "t";
    temp += to_string(temp_count++);
    IT->idTable[temp] = string();
    quadruple->addQuadruple(op, calculate(quadruple, lhs), calculate(quadruple, rhs), temp);
    return temp;
}