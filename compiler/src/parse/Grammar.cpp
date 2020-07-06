#include <iostream>
#include <vector>
#include <string.h>
#include "lex.h"
#include "Grammar.h"
using namespace std;
Token currenttoken;
int VariableOutcome;///////////////////加报错输出
int a;
Token temp;
void GrammarAnalysis(Token token) {
    currenttoken=token;
    a = ExceptStructOrArray();
    temp = currenttoken;//int处
    if (a == 2) {
        ArraySetting();
    } else if (a == 1) {
        //别的类型
        currenttoken = get_token();
        if (currenttoken.token_type==TK_IDENT) {
            currenttoken=get_token();
            if (currenttoken.token_type== TK_LEFTPAR) {
                Function();
            }
            else {
                VariableSetting();
            }
        } else if (currenttoken.token_type == TK_MUL) {
            while ((currenttoken = get_token()).token_type== TK_MUL) {
                continue;
            }
            if (currenttoken.token_type==TK_IDENT) {
                if ((currenttoken = get_token()).token_type== TK_LEFTPAR) {
                    Function();
                } else {
                    PointSetting();
                }
            }
        }
        else{
            cout<<"Wrong type!1"<<endl;
            //终止
        }
    } else if (a == 0) {//struct开头
        currenttoken = get_token();
        if (currenttoken.token_type==TK_IDENT) {
            currenttoken=get_token();
            if (currenttoken.token_type== TK_LEFTBRACE) {
                //为{，开始struct声明
                StructNaming();
            } else if (currenttoken.token_type==TK_IDENT) {
                currenttoken = get_token();
                if (currenttoken.token_type == TK_LEFTPAR) {
                    Function();
                }
                    else{
                    StructSetting();
                    }
                }
                else{
                    cout << "Wrong StructSetting!2" << endl;
                //终止
                }
            } else if (currenttoken.token_type == TK_MUL) {
                while ((currenttoken = get_token()).token_type== TK_MUL) {
                    continue;
                }
                if (currenttoken.token_type==TK_IDENT) {
                    if ((currenttoken = get_token()).token_type== TK_LEFTPAR) {
                        Function();
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
    if (temp.token_type == TK_NUMBERTYPE) {
        //number
        NumberSetting();
    } else if (temp.token_type == TK_STRINGTYPE) {
        //string
        StringSetting();
    } else if (temp.token_type==TK_CHARTYPE) {
        //char
        CharSetting();
    } else if (temp.token_type==TK_BOOL) {
        //bool
        BoolSetting();
    }
    else{
        cout<<"Wrong VariableSetting!5"<<endl;
        //终止
    }
}

int VariableName() {
    if (currenttoken.token_type == TK_IDENT) {
        return 1;
    } else {cout << "Wrong VariableName!6" << endl;
    //终止
    }
}
void StructNaming() {
    while ((currenttoken = get_token()).token_type != TK_RIGHTBRACE) {
        a = ExceptStructOrArray();
        temp = currenttoken;
        if (a == 2) {//数组定义
            ArraySetting();
        }
        else if(a==1){
            currenttoken = get_token();
            if (currenttoken.token_type==TK_IDENT) {
                if ((currenttoken = get_token()).token_type== TK_LEFTPAR) {
                    IfParameter();
                } else {
                    VariableSetting();
                }
            }
            else if (currenttoken.token_type == TK_MUL) {
                while ((currenttoken = get_token()).token_type== TK_MUL) {
                    continue;
                }
                if (currenttoken.token_type==TK_IDENT) {
                    if ((currenttoken = get_token()).token_type== TK_LEFTPAR) {
                        IfParameter();
                    } else {
                        PointSetting();
                    }
                }
            }
            else{
                cout<<"Wrong type!7"<<endl;
                //终止
            }
        }
        else if(a==0){//struct开头
            currenttoken = get_token();
            if (currenttoken.token_type==TK_IDENT) {//类名
                currenttoken=get_token();
                  if (currenttoken.token_type==TK_IDENT) {//变量名
                    currenttoken = get_token();
                    if (currenttoken.token_type == TK_LEFTPAR) {
                        IfParameter();
                    }
                    else{
                        StructSetting();
                    }
                }
            else if (currenttoken.token_type == TK_MUL) {
                while ((currenttoken = get_token()).token_type== TK_MUL) {
                    continue;
                }
                if (currenttoken.token_type==TK_IDENT) {
                    if ((currenttoken = get_token()).token_type== TK_LEFTPAR) {
                        IfParameter();
                    } else {
                        PointSetting();
                    }
                }
                else{
                    cout << "Wrong StructNaming!8" << endl;
                    //终止
                }
            } else {
                cout << "Wrong StructNaming!9" << endl;
                //终止
            }
        }
            else{
                cout << "Wrong StructNaming!10" << endl;
                //终止
            }
    }
}
    if((currenttoken=get_token()).token_type!=TK_SEMICOLON){
        cout << "Wrong StructNaming!11" << endl;
        //终止
    }
}

void NumberSetting() {
    if(currenttoken.token_type==TK_ASSIGN){
        currenttoken=get_token();
     if(currenttoken.token_type==TK_NUM||currenttoken.token_type==TK_IDENT){
         if((currenttoken=get_token()).token_type!=TK_SEMICOLON) {
             cout << "Wrong number setting101!" << endl;
             //终止
         }
         }
     else{
         cout << "Wrong number setting121!" << endl;
         //终止
     }
    }
    else if(currenttoken.token_type==TK_COMMA){
    while (currenttoken.token_type== TK_COMMA) {
        currenttoken = get_token();
        if ((VariableOutcome = VariableName()) != 1) {
            cout << "Wrong number setting!12" << endl;
            //终止
        }
        currenttoken=get_token();
    }
    if (currenttoken.token_type == TK_ASSIGN) {
        currenttoken=get_token();
        if (currenttoken.token_type==TK_NUM||currenttoken.token_type==TK_IDENT) {
            if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
                cout << "Wrong number setting!13" << endl;
                //终止
            }
        } else {
            cout << "Wrong number setting!14" << endl;
            //终止
        }
    } else if (currenttoken.token_type != TK_SEMICOLON) {
        cout << "Wrong number setting!15" << endl;
        //终止
    }
}
    else if(currenttoken.token_type!=TK_SEMICOLON){
        cout << "Wrong number setting!103" << endl;
        //终止
    }
}

void StringSetting() {
    if(currenttoken.token_type==TK_ASSIGN){
        currenttoken=get_token();
        if(currenttoken.token_type==TK_STR||currenttoken.token_type==TK_IDENT){
            if((currenttoken=get_token()).token_type!=TK_SEMICOLON) {
                cout << "Wrong String setting101!" << endl;
                //终止
            }
        }
        else{
            cout << "Wrong String setting101!" << endl;
            //终止
        }
    }
    else if(currenttoken.token_type==TK_COMMA){
    while (currenttoken.token_type== TK_COMMA) {
        currenttoken = get_token();
        if ((VariableOutcome = VariableName()) != 1) {
            cout << "Wrong string setting!16" << endl;
            //终止
        }
        currenttoken=get_token();
    }
    if (currenttoken.token_type == TK_ASSIGN) {
        currenttoken=get_token();
        if (currenttoken.token_type==TK_STR||currenttoken.token_type==TK_IDENT) {
            if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
                cout << "Wrong string setting!17" << endl;
                //终止
            }
        } else {
            cout << "Wrong string setting!18" << endl;
            //终止
        }
    }
    else if(currenttoken.token_type!=TK_SEMICOLON){
        cout << "Wrong string setting!113" << endl;
        //终止
    }
    }
    else if (currenttoken.token_type != TK_SEMICOLON) {
        cout << "Wrong string setting!19" << endl;
        //终止
    }
}

void CharSetting() {
    if(currenttoken.token_type==TK_ASSIGN){
        currenttoken=get_token();
        if(currenttoken.token_type==TK_CHAR||currenttoken.token_type==TK_IDENT){
            if((currenttoken=get_token()).token_type!=TK_SEMICOLON) {
                cout << "Wrong char setting102!" << endl;
                //终止
            }
        }
        else{
            cout << "Wrong char setting103!" << endl;
            //终止
        }
    }
    else if(currenttoken.token_type==TK_COMMA) {
        while (currenttoken.token_type == TK_COMMA) {
            currenttoken = get_token();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong char setting!20" << endl;
                //终止
            }
            currenttoken = get_token();
        }
        if (currenttoken.token_type == TK_ASSIGN) {
            currenttoken=get_token();
            if (currenttoken.token_type==TK_CHAR||currenttoken.token_type==TK_IDENT) {
                if ((currenttoken = get_token()).token_type != TK_SEMICOLON) {
                    cout << "Wrong char setting!21" << endl;
                    //终止
                }
            } else {
                cout << "Wrong char setting!22" << endl;
                //终止
            }
        }
        else if(currenttoken.token_type!=TK_SEMICOLON){
            cout << "Wrong char setting!112" << endl;
            //终止
        }
    }else if (currenttoken.token_type != TK_SEMICOLON) {
        cout << "Wrong char setting!23" << endl;
        //终止
    }
}

void BoolSetting() {
    if(currenttoken.token_type==TK_ASSIGN){
        currenttoken=get_token();
        if(currenttoken.token_type==TK_TRUE||currenttoken.token_type==TK_FALSE||currenttoken.token_type==TK_IDENT){
            if((currenttoken=get_token()).token_type!=TK_SEMICOLON) {
                cout << "Wrong bool setting101!" << endl;
                //终止
            }
        }
        else{
            cout << "Wrong bool setting101!" << endl;
            //终止
        }
    }
    else if(currenttoken.token_type==TK_COMMA) {
        while (currenttoken.token_type == TK_COMMA) {
            currenttoken = get_token();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong bool setting!24" << endl;
                //终止
            }
            currenttoken = get_token();
        }
        if (currenttoken.token_type == TK_ASSIGN) {
            currenttoken=get_token();
            if (currenttoken.token_type==TK_TRUE||currenttoken.token_type==TK_FALSE||currenttoken.token_type==TK_IDENT){
                if ((currenttoken = get_token()).token_type != TK_SEMICOLON) {
                    cout << "Wrong bool setting!25" << endl;
                    //终止
                }
            } else {
                cout << "Wrong bool setting!26" << endl;
                //终止
            }
        }
        else if(currenttoken.token_type!=TK_SEMICOLON){
            cout << "Wrong bool setting!111" << endl;
            //终止
        }
    }
    else if (currenttoken.token_type != TK_SEMICOLON) {
        cout << "Wrong bool setting!27" << endl;
        //终止
    }
}

void ArraySetting() {
    if ((currenttoken = get_token()).token_type== TK_LEFTBRKT) {
        if ((currenttoken = get_token()).token_type== TK_NUM) {
            if ((currenttoken = get_token()).token_type== TK_RIGHTBRKT) {
                if ((currenttoken = get_token()).token_type!= TK_OF) {
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
    while ((currenttoken = get_token()).token_type== TK_ARRAY) {
        if ((currenttoken = get_token()).token_type== TK_LEFTBRKT) {
            if ((currenttoken = get_token()).token_type== TK_NUM) {
                if ((currenttoken = get_token()).token_type== TK_RIGHTBRKT) {
                    if ((currenttoken = get_token()).token_type!= TK_OF) {
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
    if (currenttoken.token_type== TK_NUMBERTYPE || currenttoken.token_type== TK_CHARTYPE ||
        currenttoken.token_type== TK_STRINGTYPE || currenttoken.token_type== TK_BOOL) {
        if ((currenttoken = get_token()).token_type== TK_IDENT) {
            if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
                cout << "Wrong array setting!36" << endl;
                //终止
            }
        } else {
            cout << "Wrong array setting!37" << endl;
            //终止
        }
    } else if (currenttoken.token_type== TK_STRUCT) {
        if ((currenttoken = get_token()).token_type== TK_IDENT) {
            if ((currenttoken = get_token()).token_type== TK_IDENT) {
                if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
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
    if(currenttoken.token_type==TK_ASSIGN){
        currenttoken=get_token();
        if (currenttoken.token_type == TK_NUM || currenttoken.token_type == TK_CHAR ||
            currenttoken.token_type == TK_STR || currenttoken.token_type == TK_IDENT ||
            currenttoken.token_type==TK_TRUE || currenttoken.token_type == TK_FALSE) {
            if ((currenttoken = get_token()).token_type != TK_SEMICOLON) {
                cout << "Wrong struct setting!43" << endl;
                //终止
            }
        }
        else{
            cout << "Wrong struct setting!43" << endl;
            //终止
        }
    }
    else if(currenttoken.token_type==TK_COMMA) {
        while (currenttoken.token_type == TK_COMMA) {
            currenttoken = get_token();
            if (currenttoken.token_type==TK_IDENT) {
                currenttoken = get_token();
            } else {
                cout << "Wrong struct setting!42" << endl;
                //终止
            }
        }
        if (currenttoken.token_type == TK_ASSIGN) {
            //判断是否属于值
            currenttoken=get_token();
            if (currenttoken.token_type == TK_NUM || currenttoken.token_type == TK_CHAR ||
                currenttoken.token_type == TK_STR || currenttoken.token_type == TK_IDENT ||
                currenttoken.token_type == TK_TRUE || currenttoken.token_type == TK_FALSE) {
                if ((currenttoken = get_token()).token_type != TK_SEMICOLON) {
                    cout << "Wrong struct setting!43" << endl;
                    //终止
                }
            } else {
                cout<<"Wrong struct setting!43"<<endl;
                //终止
            }
        }
        else if(currenttoken.token_type!=TK_SEMICOLON){
            cout<<"Wrong struct setting!110"<<endl;
            //终止
        }
    }else if (currenttoken.token_type != TK_SEMICOLON) {
        cout << "Wrong struct setting!44" << endl;
        //终止
    }
}

void PointSetting() {
    if(currenttoken.token_type==TK_ASSIGN){
        currenttoken=get_token();
        if(currenttoken.token_type==TK_IDENT){
            if((currenttoken=get_token()).token_type!=TK_SEMICOLON){
                cout<<"Wrong Point setting! 106"<<endl;
                //终止
            }
        }
        else if(currenttoken.token_type==TK_AND){
            if((currenttoken=get_token()).token_type==TK_IDENT){
                if((currenttoken=get_token()).token_type!=TK_SEMICOLON){
                    cout<<"Wrong Point setting! 106"<<endl;
                    //终止
                }
            }
            else{
                cout<<"Wrong Point setting! 107"<<endl;
                //终止
            }
        }
        else{
            cout<<"Wrong Point setting! 108"<<endl;
            //终止
        }
    }
    else if(currenttoken.token_type==TK_COMMA) {
        while (currenttoken.token_type == TK_COMMA) {
            currenttoken = get_token();
            if (currenttoken.token_type==TK_IDENT) {
                currenttoken = get_token();
            } else {
                cout << "Wrong Point setting!45" << endl;
                //终止
            }
        }
        if (currenttoken.token_type == TK_ASSIGN) {
            currenttoken=get_token();
            if ((currenttoken = get_token()).token_type == TK_AND) {
                if ((currenttoken = get_token()).token_type != TK_IDENT) {
                    cout << "Wrong Point setting!46" << endl;
                    //终止
                }
            }
            else if(currenttoken.token_type==TK_IDENT){
                if((currenttoken=get_token()).token_type!=TK_SEMICOLON){
                    cout<<"Wrong Point setting! 109"<<endl;
                    //终止
                }
            }
        }
        else if (currenttoken.token_type != TK_SEMICOLON) {
            cout << "Wrong Point setting!48" << endl;
            //终止
        }
    }
    else if(currenttoken.token_type!=TK_SEMICOLON){
        cout << "Wrong Point setting!109" << endl;
        //终止
    }
    }

void Function() {
    currenttoken = get_token();
    if (currenttoken.token_type == TK_RIGHTPAR) {
        currenttoken=get_token();
        if (currenttoken.token_type== TK_LEFTBRACE) {
            while((currenttoken=get_token()).token_type!=TK_RIGHTBRACE) {
                ControlStream();
            }
        } else if(currenttoken.token_type!=TK_SEMICOLON) {
            cout << "Wrong function!49" << endl;
            //终止
        }
    } else if(currenttoken.token_type==TK_NUMBERTYPE||currenttoken.token_type==TK_STRINGTYPE||currenttoken.token_type==TK_CHARTYPE||currenttoken.token_type==TK_BOOL||currenttoken.token_type==TK_STRUCT) {
        Parameter();
        while ((currenttoken = get_token()).token_type == TK_COMMA){
            currenttoken=get_token();
            Parameter();
        }
        if(currenttoken.token_type==TK_RIGHTPAR){
            if((currenttoken=get_token()).token_type==TK_LEFTBRACE){
                while((currenttoken=get_token()).token_type!=TK_RIGHTBRACE) {
                    ControlStream();
                }
            }
            else if(currenttoken.token_type!=TK_SEMICOLON) {
                cout << "Wrong function!50" << endl;
                //终止
            }
            }
        else{
            cout << "Wrong function!51" << endl;
            //终止
        }
    }else{
        cout << "Wrong function!52" << endl;
    //终止
    }
}

void ControlStream() {
    if (currenttoken.token_type== TK_NUMBERTYPE || currenttoken.token_type== TK_CHARTYPE ||
        currenttoken.token_type== TK_STRINGTYPE || currenttoken.token_type== TK_BOOL ||
        currenttoken.token_type== TK_STRUCT) {
        a = ExceptStructOrArray();
        temp = currenttoken;
        if (a == 2) {
            ArraySetting();
        } else if (a == 1) {
            //别的类型
            currenttoken = get_token();
            if (currenttoken.token_type==TK_IDENT) {
                VariableSetting();
            } else if (currenttoken.token_type == TK_MUL) {
                while ((currenttoken = get_token()).token_type== TK_MUL) {
                    continue;
                }
                if (currenttoken.token_type==TK_IDENT) {
                    PointSetting();
                }
            }
        } else if (a == 0) {//struct开头
            currenttoken = get_token();
            if (currenttoken.token_type==TK_IDENT) {//结构体名
                currenttoken = get_token();
                if (currenttoken.token_type==TK_IDENT) {//变量&函数名
                    StructSetting();
                } else if (currenttoken.token_type == TK_MUL) {
                    while ((currenttoken = get_token()).token_type== TK_MUL) {
                        continue;
                    }
                    if (currenttoken.token_type==TK_IDENT) {
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
    } else if (currenttoken.token_type==TK_IDENT) {
        currenttoken = get_token();
        if (currenttoken.token_type == TK_LEFTPAR) {
            if (currenttoken.token_type == TK_RIGHTPAR) {//无参数
                if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
                    cout << "Wrong function!56" << endl;
                    //终止
                } else
                    currenttoken = get_token();
            } else if ((VariableOutcome = IfType()) == 1) {//有参数
                currenttoken = get_token();
                if (currenttoken.token_type==TK_IDENT) {
                    while ((currenttoken = get_token()).token_type== TK_COMMA) {
                        currenttoken = get_token();
                        if ((VariableOutcome = IfType()) == 1) {
                            currenttoken = get_token();
                            if ((VariableOutcome = VariableName()) != 1) {
                                cout << "Wrong function!57" << endl;
                                //终止
                            }
                        } else
                            cout << "Wrong function!58" << endl;
                        //终止
                    }
                    if (currenttoken.token_type == TK_RIGHTPAR) {
                        if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
                            cout << "Wrong function59!" << endl;
                            //终止
                        } else currenttoken = get_token();
                    } else
                        cout << "Wrong function!60" << endl;
                    //终止
                } else
                    cout << "Wrong function!61" << endl;
                //终止
            } else
                cout << "Wrong function!62" << endl;
            //终止
        }
        while (currenttoken.token_type == TK_COMMA) {//赋值
            currenttoken = get_token();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong function!63" << endl;
                //终止
            }
            currenttoken = get_token();
        }
        if (currenttoken.token_type == TK_ADD) {
            if ((currenttoken = get_token()).token_type!= TK_ASSIGN) {
                cout << "Wrong function!64" << endl;
                //终止
            }
            while(currenttoken.token_type!=TK_SEMICOLON) {
                Expression();
            }
        } else if (currenttoken.token_type == TK_SUB) {
            if ((currenttoken = get_token()).token_type!= TK_ASSIGN) {
                cout << "Wrong function!65" << endl;
                //终止
            }
            while(currenttoken.token_type!=TK_SEMICOLON) {
                Expression();
            }
        } else if (currenttoken.token_type == TK_ASSIGN) {
            while(currenttoken.token_type!=TK_SEMICOLON) {
                Expression();
            }
        } else if(currenttoken.token_type!=TK_SEMICOLON){
            cout << "Wrong function!66" << endl;
        //终止
        }
    } else if (currenttoken.token_type == TK_BREAK) {
        if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
            cout << "Wrong function!68" << endl;
            //终止
        }
    } else if (currenttoken.token_type == TK_CONTINUE) {
        if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
            cout << "Wrong function!69" << endl;
            //终止
        }
    } else if (currenttoken.token_type == TK_RETURN) {
        Expression();
        if ((currenttoken = get_token()).token_type!= TK_SEMICOLON) {
            cout << "Wrong function!70" << endl;
            //终止
        }
    } else if (currenttoken.token_type == TK_IF) {
        IfExpression();
    } else if (currenttoken.token_type == TK_WHILE) {
        WhileExpression();
    } else
        cout << "Wrong function!71" << endl;
    //终止
}

void Expression() {
    currenttoken = get_token();
}

void IfExpression() {
    if ((currenttoken = get_token()).token_type== TK_LEFTPAR) {
        while(currenttoken.token_type!=TK_RIGHTPAR){
        Expression();
        }
            if ((currenttoken = get_token()).token_type== TK_LEFTBRACE) {
                ControlStream();
                if (currenttoken.token_type == TK_RIGHTBRACE) {
                    if ((currenttoken = get_token()).token_type== TK_ELSE) {
                        if ((currenttoken = get_token()).token_type== TK_IF) {
                            IfExpression();
                        } else if ((currenttoken = get_token()).token_type== TK_LEFTBRACE) {
                            while ((currenttoken = get_token()).token_type!= TK_RIGHTBRACE) {
                                ControlStream();
                            }
                        }
                        else{
                            cout << "Wrong If function!100" << endl;
                            //终止
                        }
                    }
                }
            } else
                cout << "Wrong If function!72" << endl;
            //终止
    } else
        cout << "Wrong If function!74" << endl;
    //终止
}

void WhileExpression() {
    if ((currenttoken = get_token()).token_type== TK_LEFTPAR) {
        while(currenttoken.token_type!=TK_RIGHTPAR) {
            Expression();
        }
            if ((currenttoken = get_token()).token_type== TK_LEFTBRACE) {
                while ((currenttoken = get_token()).token_type!= TK_RIGHTBRACE) {
                    ControlStream();
                }
            }
            cout << "Wrong While function!75" << endl;
            //终止
    }
    cout << "Wrong While function!77" << endl;
    //终止
}
int  ExceptStructOrArray(){
    if(currenttoken.token_type==TK_STRUCT){
        return 0;
    }
    else if(currenttoken.token_type==TK_ARRAY){
        return 2;
    }
    else if(currenttoken.token_type==TK_NUMBERTYPE||currenttoken.token_type==TK_CHARTYPE||currenttoken.token_type==TK_STRINGTYPE||currenttoken.token_type==TK_BOOL){
        return 1;
    }
    else{
        cout<<"Wrong Type!78"<<endl;
        //终止
    }
}
int IfType(){
    if(currenttoken.token_type==TK_NUMBERTYPE||currenttoken.token_type==TK_CHARTYPE||currenttoken.token_type==TK_STRINGTYPE||currenttoken.token_type==TK_BOOL){
        return 1;
    }
    else {
        cout<<"Wrong Function!79"<<endl;
        //终止
    }
}
void IfParameter() {//函数是否有参数
    currenttoken = get_token();
    if (currenttoken.token_type == TK_RIGHTPAR) {//无参数
        if ((currenttoken = get_token()).token_type != TK_SEMICOLON) {
            cout << "Wrong function!80" << endl;
            //终止
        }
    }
    else if(currenttoken.token_type==TK_NUMBERTYPE||currenttoken.token_type==TK_STRINGTYPE||currenttoken.token_type==TK_CHARTYPE||currenttoken.token_type==TK_BOOL||currenttoken.token_type==TK_STRUCT) {
        Parameter();
        while ((currenttoken = get_token()).token_type == TK_COMMA){
            currenttoken=get_token();
            Parameter();
        }
        if(currenttoken.token_type==TK_RIGHTPAR){
            if((currenttoken=get_token()).token_type!=TK_SEMICOLON){
                cout << "Wrong function!81" << endl;
                //终止
            }
        }
        else{
            cout << "Wrong function!82" << endl;
            //终止
        }
    }
    else{
        cout << "Wrong function!83" << endl;
        //终止
    }
}
void Parameter(){//有参数
    if(currenttoken.token_type==TK_STRUCT){
        if((currenttoken=get_token()).token_type==TK_IDENT){
            currenttoken=get_token();
            if(currenttoken.token_type==TK_MUL){
                while((currenttoken=get_token()).token_type==TK_MUL){
                    continue;
                }
                if(currenttoken.token_type!=TK_IDENT){
                    cout << "Wrong function!84" << endl;
                    //终止
                }
            }
            else if(currenttoken.token_type!=TK_IDENT){
                cout << "Wrong function!85" << endl;
                //终止
            }
        }
        else{
            cout << "Wrong function!86" << endl;
            //终止
        }
    }
    else if((VariableOutcome=IfType())==1){
        currenttoken=get_token();
        if(currenttoken.token_type==TK_MUL){
            while((currenttoken=get_token()).token_type==TK_MUL){
                continue;
            }
            if(currenttoken.token_type!=TK_IDENT){
                cout << "Wrong function!87" << endl;
                //终止
            }
        }
        else if(currenttoken.token_type!=TK_IDENT){
            cout << "Wrong function!88" << endl;
            //终止
        }
    }
}
