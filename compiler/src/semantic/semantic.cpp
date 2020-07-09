// parse.h向前类声明出错

#include "semantic.h"

auto CT = new constTable();
auto IT = new idenTable();

// double转char*
char* doubleToStr( double n ){
    string s = to_string(n);
    const char *p = s.c_str();
    return const_cast<char *>(p);
}

// 字符串(name)转double
double strToDouble( const char *s ){
    double ret=0;
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
    return ret;
}

// 遍历树 获取一个结点的数值同时更新四元式
// 未创建临时变量
double getNumber( Expression_Statement_node *p ){
    if( p->nodetype == ND_NUM ){
        // 如果是数值
        return strToDouble( p->getName() );
    } else if( p->nodetype == ND_VARREF ) {
        // 变量
        return IT->getValue(const_cast<char *>(p->getName()));
    } else {
        // 算术符号
        if( !strcmp( p->getName(),"*" ) ){
            // 乘号
            return getNumber( p->lhs ) * getNumber(p->rhs);
        } else if( !strcmp( p->getName(),"/" ) ){
            // 除号
            return getNumber( p->lhs ) / getNumber(p->rhs);
        } else if( !strcmp( p->getName(),"+" ) ){
            // 加号
            return getNumber( p->lhs ) + getNumber(p->rhs);
        } else if( !strcmp( p->getName(),"-" ) ){
            // 减号
            return getNumber( p->lhs ) - getNumber(p->rhs);
        }
    }
}

// 赋值语句 当前p结点为_equ
void Assignment( Quadruple *quadruple, Expression_Statement_node *p ){
    Expression_Statement_node *lhs = p->lhs;
    Expression_Statement_node *rhs = p->rhs;
    double value=0;
    if( IT->exist((char *) lhs->getName()) ){
        value = getNumber( rhs );
    } else {
        std::cout<<"Identifier not found in IT"<<std::endl;
        return;
    }
    // 生成四元式
    quadruple->addQuadruple("=",doubleToStr(value),"_",lhs->getName());
    // 标识符表赋值
    IT->idTable[ const_cast<char*>(lhs->getName()) ]=value;
}

// 递归 函数定义
void FuncDeclare( Quadruple *quadruple, Declaration_node *p ){
    quadruple->addQuadruple( "function","","",p->getName() );
    traverse(quadruple,p);
}

// Return语句
void RT( Quadruple *quadruple, Expression_Statement_node *p ){
    double retValue = getNumber(dynamic_cast<Expression_Statement_node *>(p->returnval));
    quadruple->addQuadruple( "return","","", doubleToStr(retValue) );
}

void IF( Quadruple *quadruple, Expression_Statement_node *p ){
    // IF语句
    Expression_Statement_node *cond = dynamic_cast<Expression_Statement_node *>(p->cond);
    if( cond->nodetype == ND_EQ ){
        // 等于

    } else if( cond->nodetype == ND_LE ){
        //小于等于
    } else if( cond->nodetype == ND_GE ){
        //大于等于
    } else if( cond->nodetype == ND_LOGAND ){
        // AND
    } else if( cond->nodetype == ND_LOGOR ){
        // OR
    }

}

void WH( Quadruple *quadruple, Expression_Statement_Node *p ){
    // WH语句
    Expression_Statement_node *cond = dynamic_cast<Expression_Statement_node *>(p->cond);

}

//遍历树更新四元式
void traverse( Quadruple *quadruple, Nodebase *p ){
    for(int i=0; i < p->stmts.size(); i++){
        Nodebase *cur = p->stmts[i];
        Expression_Statement_node *statementNode;
        Declaration_node *declarationNode;
        if( cur->nodetype == ND_DEC ){
            // 声明结点
            for(int j=0; j<cur->stmts.size(); j++){
                // push label
                IT->append(const_cast<char *>(cur->stmts[j]->getName()));
            }
        } else if( cur->nodetype == ND_ASSIGN ){
            // 赋值结点
            statementNode = dynamic_cast<Expression_Statement_node*>(p);
            Assignment( quadruple,statementNode );
        } else if( cur->nodetype == ND_FUNC ){
            // 函数定义
            declarationNode = dynamic_cast<Declaration_node*>(p);
            FuncDeclare( quadruple,declarationNode );
        } else if( cur->nodetype == ND_RETURN ){
            // 函数return
            statementNode = dynamic_cast<Expression_Statement_node*>(p);
            if( !statementNode->returnval){
                // 函数没有返回值
                quadruple->addQuadruple( "return","","","void" );
            } else {
                // 函数有返回值
                RT( quadruple,statementNode );
            }
        } else if( cur->nodetype == ND_IF ){
            // IF
            statementNode = dynamic_cast<Expression_Statement_node*>(p);
            IF( quadruple,statementNode );
        } else if ( cur->nodetype == ND_DO_WHILE ){

        }
    }
}

Quadruple *treeToQuad( Nodebase *p ){
    auto quadruple = new Quadruple();

    if( p->nodetype != ND_PROG ){
        cout<<"Root Node is not Program!";
        return nullptr;
    } else {
        // 遍历program树生成四元式
        traverse(quadruple,p);
    }
    return quadruple;
}