//// parse.h向前类声明出错
//
//#include "semantic.h"
//
//// double转char*
//char* doubleToStr( double n ){
//    string s = to_string(n);
//    const char *p = s.c_str();
//    return const_cast<char *>(p);
//}
//
//// 字符串(name)转double
//double strToDouble( const char *s ){
//    double ret=0;
//    double afterDot = 0.1;
//    for(int i=0; i<strlen(s) ; i++){
//        if( s[i] != '.' ){
//            // 整数部分
//            ret = ret*10 + s[i] - '0';
//        } else {
//            // 小数部分
//            for(int j=i+1; j<strlen(s); j++){
//                ret += (s[j] -'0') * afterDot;
//                afterDot *= afterDot;
//            }
//            break;
//        }
//    }
//    return ret;
//}
//
//// 遍历树 获取一个结点的数值同时更新四元式
//// 未创建临时变量
//double getNumber( Quadruple *quadruple,Expression_Statement_Node *p,idenTable *IT ){
//    if( p->NODE_TYPE == ND_NUM ){
//        // 如果是数值
//        return strToDouble( p->name );
//    } else if( p->NODE_TYPE == ND_VARREF ) {
//        // 变量
//        return IT->getValue( p->name );
//    } else {
//        // 算术符号
//        if( !strcmp( p->name,"*" ) ){
//            // 乘号
//            //quadruple->addQuadruple( ND_MUL, p->lhs->getName(), p->rhs->getName(), )
//            return getNumber( quadruple, p->lhs,IT ) * getNumber(quadruple, p->rhs,IT);
//        } else if( !strcmp( p->name,"/" ) ){
//            // 除号
//            return getNumber( quadruple, p->lhs,IT ) / getNumber(quadruple, p->rhs,IT);
//        } else if( !strcmp( p->name,"+" ) ){
//            // 加号
//            return getNumber( quadruple, p->lhs,IT ) + getNumber(quadruple, p->rhs,IT);
//        } else if( !strcmp( p->name,"-" ) ){
//            // 减号
//            return getNumber( quadruple, p->lhs,IT ) - getNumber(quadruple, p->rhs,IT);
//        }
//    }
//}
//
//// 赋值语句 当前p结点为_equ
//void Assignment( Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT ){
//    Expression_Statement_Node *lhs = p->lhs;
//    Expression_Statement_Node *rhs = p->rhs;
//    double value=0;
//    if( IT->exist( lhs->getName ) ){
//        value = getNumber( rhs );
//    } else {
//        cout<<"Identifier not found in IT"<<endl;
//        return;
//    }
//    // 生成四元式
//    quadruple->addQuadruple(ND_ASSIGN,doubleToStr(value),"_",lhs->getName);
//    // 标识符表赋值
//    IT[lhs->getName] = value;
//}
//
//// 递归 函数定义
//void FuncDeclare( Quadruple *quadruple, Declaration_node *p, constTable *CT, idenTable *IT ){
//    quadruple->addQuadruple( ND_FUNC,"","",p->getName() );
//    traverse(quadruple,p,CT,IT);
//}
//
//// Return语句
//void RT( Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT ){
//    double  retValue = getNumber(quadruple, reinterpret_cast<Expression_Statement_Node *>(p), IT);
//    quadruple->addQuadruple( ND_RETURN,"","", doubleToStr(retValue) );
//}
//
//void IF( Quadruple *quadruple, Expression_Statement_Node *p, constTable *CT, idenTable *IT ){
//    //
//}
//
////遍历树更新四元式
//void traverse( Quadruple *quadruple, Nodebase *p, constTable *CT, idenTable *IT ){
//    for(int i=0; i < p->stmts.size(); i++){
//        Nodebase *cur = p->stmts[i];
//        Expression_Statement_Node *statementNode;
//        Declaration_node *declarationNode;
//        if( cur->nodetype == ND_DEC ){
//            // 声明结点
//            for(int j=0; j<cur->stmts.size(); j++){
//                // push label
//                IT->append(const_cast<char *>(cur->stmts[j]->getName()));
//            }
//        } else if( cur->nodetype == ND_ASSIGN ){
//            // 赋值结点
//            statementNode = static_cast<Expression_Statement_Node*>(p);
//            Assignment( quadruple,statementNode,CT,IT );
//        } else if( cur->nodetype == ND_FUNC ){
//            // 函数定义
//            declarationNode = dynamic_cast<Declaration_node*>(p);
//            FuncDeclare( quadruple,declarationNode,CT,IT );
//        } else if( cur->nodetype == ND_RETURN ){
//            // 函数return
//            statementNode = static_cast<Expression_Statement_Node*>(p);
//            if( statementNode->returnval = nullptr ){
//                // 函数没有返回值
//                quadruple->addQuadruple( ND_RETURN,"","",statementNode->retValue );
//            } else {
//                // 函数有返回值
//                RT( quadruple,statementNode,CT,IT );
//            }
//        } else if( cur->nodetype == ND_IF ){
//            // IF
//            statementNode = static_cast<Expression_Statement_Node*>(p);
//            IF( quadruple,statementNode,CT,IT );
//        }
//    }
//}
//
//Quadruple *treeToQuad(constTable *CT, idenTable *IT){
//    Nodebase *p = get_AST();
//    auto quadruple = new Quadruple();
//
//    if( p->nodetype != ND_PROG ){
//        cout<<"Root Node is not Program!";
//        return nullptr;
//    } else {
//        // 遍历program树生成四元式
//        traverse(quadruple,p, CT, IT);
//    }
//    return quadruple;
//}