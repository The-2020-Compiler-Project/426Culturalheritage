#include <iostream>
#include <vector>
#include <string.h>
using namespace std;
string currenttoken;
string get_token();//得到当前单个token字符
bool GrammarAnalysis();//语法分析
void VariableSetting();//变量声明
int  ExceptStruct();//判断是否为除struct以外的类型
int VariableName();//变量名
void StructNaming();//struct声明
void NumberSetting();
void StringSetting();
void CharSetting();
void BoolSetting();
void StructSetting();
void ArraySetting();
void PointSetting();
void Function();
int IfType();
void IoStream();
void Expression();
void IfExpression();
void WhileExpression();
int VariableOutcome;
string temp;
bool GrammarAnalysis(){
            //得到当前token值作为参数！！！
            int a=ExceptStruct();
            temp=currenttoken;
            if(a==1){
                //别的类型
                currenttoken=get_token();
                if((VariableOutcome=VariableName())==1){
                    if((currenttoken=get_token())=="("){
                        Function();
                    }
                    else if(temp=="类型"){
                        //变量声明
                        if() {
                            currenttoken = get_token();
                            if (currenttoken == "*") {
                                //指针
                            }
                            else if ((VariableOutcome = VariableName()) == 1) {
                                while ((currenttoken = get_token()) == ",") {
                                    currenttoken=get_token();
                                    if ((VariableOutcome = VariableName()) != 1) {
                                        cout << "Wrong number setting!" << endl;
                                        //终止
                                    }
                                }
                                if (currenttoken != "=") {
                                    cout << "Wrong number setting!" << endl;
                                    //终止
                                }
                                if ((currenttoken = get_token()) != "shuzi") {
                                    cout << "Wrong number setting!" << endl;
                                    //终止
                                }
                                if ((currenttoken = get_token()) != ";") {
                                    cout << "Wrong number setting!" << endl;
                                    //终止
                                }
                            }
                            else
                                cout<<"Wrong Variable setting!"<<endl;
                            //终止
                        }
                        else if () {
                            //string
                            StringSetting();
                        }
                        else if(){
                            //char
                            CharSetting();
                        }
                        else if(){
                            //bool
                            BoolSetting();
                        }
                        else if(){
                            //数组，读array
                            ArraySetting();
                        }
                        else if(currenttoken=="struct"){
                            currenttoken=get_token();
                            if ((VariableOutcome = VariableName()) != 1){
                                cout<<"Wrong struct setting!"<<endl;
                                //终止
                            }
                            else StructSetting();
                        }
                    }
                }
            }
            else if(a==0) {
                currenttoken = get_token();
                if ((VariableOutcome = VariableName()) == 1) {
                    if ((currenttoken = get_token()) == "{") {
                        //为{，开始struct声明
                       StructNaming();
                    }
                    else if ((VariableOutcome = VariableName()) == 1) {
                        currenttoken=get_token();
                            if (currenttoken=="(") {
                                Function();
                            } else if((VariableOutcome = VariableName()) == 1){
                                StructSetting();
                            }
                            else
                                cout << "Wrong StructSetting!" << endl;
                        //终止
                    }
                    else
                        cout << "Wrong StructSetting!" << endl;
                    //终止
                }
                else
                    cout << "Wrong StructSetting!" << endl;
                //终止
            }
            else
                cout<<"Wrong struct setting!"<<endl;
                //终止
        }
void VariableSetting() {
    if() {
        currenttoken = get_token();
        if (currenttoken == "*") {
            while((currenttoken=get_token())!="*"){
                PointSetting();
            }
        }
        else if ((VariableOutcome = VariableName()) == 1) {
            NumberSetting();
        }
        else
            cout<<"Wrong Variable setting!"<<endl;
        //终止
    }
    else if () {
        //string
        StringSetting();
    }
    else if(){
        //char
        CharSetting();
    }
    else if(){
        //bool
        BoolSetting();
    }
    else if(){
        ArraySetting();
    }
    else if(currenttoken=="struct"){
        currenttoken=get_token();
        if ((VariableOutcome = VariableName()) != 1){
            cout<<"Wrong struct setting!"<<endl;
            //终止
        }
        else StructSetting();
    }
}

int VariableName(){
    if(currenttoken=="biaoshifu"){
        return 1;
    }
    else cout<<"Wrong VariableName!"<<endl;
    //终止
}
void StructNaming(){
    while((currenttoken=get_token())!="}"){
        if(currenttoken=="struct"){
            currenttoken=get_token();
            if((VariableOutcome = VariableName()) == 1){
                currenttoken=get_token();
                if((VariableOutcome = VariableName()) == 1){
                    StructSetting();
                }
                else
                    cout<<"Wrong struct setting!"<<endl;
                //终止
            }
            else
                cout<<"Wrong struct setting!"<<endl;
            //终止
        }
        else
            cout<<"Wrong struct setting!"<<endl;
        //终止
    }
    if((currenttoken=get_token())!=";"){
        cout<<"Wrong struct setting!"<<endl;
        //终止
    }
}
void NumberSetting() {
        while ((currenttoken = get_token()) == ",") {
            currenttoken=get_token();
            if ((VariableOutcome = VariableName()) != 1) {
                cout << "Wrong number setting!" << endl;
                //终止
            }
        }
        if (currenttoken != "=") {
            cout << "Wrong number setting!" << endl;
            //终止
        }
        if ((currenttoken = get_token()) != "shuzi") {
            cout << "Wrong number setting!" << endl;
            //终止
        }
        if ((currenttoken = get_token()) != ";") {
            cout << "Wrong number setting!" << endl;
            //终止
        }
    }
void StringSetting(){
    currenttoken=get_token();
    if((VariableOutcome=VariableName())==1){
        while((currenttoken=get_token())==","){
            currenttoken=get_token();
            if((VariableOutcome=VariableName())!=1){
                cout<<"Wrong string setting!"<<endl;
                //终止
            }
        }
        if(currenttoken!="="){
            cout<<"Wrong string setting!"<<endl;
            //终止
        }
        if((currenttoken=get_token())!="zifuchuan"){
            cout<<"Wrong string setting!"<<endl;
            //终止
        }
        if((currenttoken=get_token())!=";"){
            cout<<"Wrong string setting!"<<endl;
            //终止
        }
    }
    else
        cout<<"Wrong string setting!"<<endl;
    //终止
}
void CharSetting(){
    currenttoken=get_token();
    if((VariableOutcome=VariableName())==1){
        while((currenttoken=get_token())==","){
            currenttoken=get_token();
            if((VariableOutcome=VariableName())!=1){
                cout<<"Wrong char setting!"<<endl;
                //终止
            }
        }
        if(currenttoken!="="){
            cout<<"Wrong char setting!"<<endl;
            //终止
        }
        if((currenttoken=get_token())!="asic"){
            cout<<"Wrong char setting!"<<endl;
            //终止
        }
        if((currenttoken=get_token())!=";"){
            cout<<"Wrong char setting!"<<endl;
            //终止
        }
    }
    else
        cout<<"Wrong char setting!"<<endl;
    //终止
}
void BoolSetting(){
    currenttoken=get_token();
    if((VariableOutcome=VariableName())==1){
        while((currenttoken=get_token())==","){
            currenttoken=get_token();
            if((VariableOutcome=VariableName())!=1){
                cout<<"Wrong bool setting!"<<endl;
                //终止
            }
        }
        if(currenttoken!="="){
            cout<<"Wrong bool setting!"<<endl;
            //终止
        }
        if((currenttoken=get_token())!="true"or"false"){
            cout<<"Wrong bool setting!"<<endl;
            //终止
        }
        if((currenttoken=get_token())!=";"){
            cout<<"Wrong bool setting!"<<endl;
            //终止
        }
    }
    else
        cout<<"Wrong bool setting!"<<endl;
    //终止
}
void ArraySetting(){
    if((currenttoken=get_token())!="["){
        cout<<"Wrong array setting!"<<endl;
        //终止
    }
    else if((currenttoken=get_token())!="shuzi"){
        cout<<"Wrong array setting!"<<endl;
        //终止
    }
    if((currenttoken=get_token())!="]"){
        cout<<"Wrong array setting!"<<endl;
        //终止
    }
    else if ((currenttoken=get_token())!="of"){
        cout<<"Wrong array setting!"<<endl;
        //终止
    }
    if(((currenttoken=get_token())!="类型")){
        cout<<"Wrong array setting!"<<endl;
        //终止
    }
    else if((currenttoken=get_token())!=";"){
        cout<<"Wrong array setting!"<<endl;
        //终止
    }
}
void StructSetting(){
        while((currenttoken=get_token())==","){
            currenttoken=get_token();
            if((VariableOutcome=VariableName())!=1){
                cout<<"Wrong struct setting!"<<endl;
                //终止
            }
        }
        if(currenttoken=="="){
            //判断是否属于值
            if(){
                if((currenttoken=get_token())!=";"){
                    cout<<"Wrong struct setting!"<<endl;
                    //终止
                }
            }
        }
        else
            cout<<"Wrong struct setting!"<<endl;
        //终止
}
void PointSetting(){
    if((VariableOutcome=VariableName())==1){
        while((currenttoken=get_token())==","){
            currenttoken=get_token();
            if((VariableOutcome=VariableName())!=1){
                cout<<"Wrong Point setting!"<<endl;
                //终止
            }
        }
        if(currenttoken=="="){
            if((currenttoken=get_token())=="&"){
                if((currenttoken=get_token())!="bianliangming"){
                    cout<<"Wrong Point setting!"<<endl;
                    //终止
                }
            }
            cout<<"Wrong Point setting!"<<endl;
            //终止
        }
        cout<<"Wrong Point setting!"<<endl;
        //终止
    }
    cout<<"Wrong Point setting!"<<endl;
    //终止
}
void Function(){
    currenttoken=get_token();
    if(currenttoken==")"){
        if((currenttoken=get_token())=="{") {
            IoStream();
        }
        else
            cout << "Wrong function!" << endl;
        //终止
    }
    else if ((VariableOutcome=IfType())==1){
            currenttoken=get_token();
            if((VariableOutcome=VariableName())==1){
                while((currenttoken=get_token())==","){
                    currenttoken=get_token();
                    if((VariableOutcome=IfType())==1){
                        currenttoken=get_token();
                        if((VariableOutcome=VariableName())!=1){
                            cout<<"Wrong function!"<<endl;
                            //终止
                        }
                    }
                    else
                        cout<<"Wrong function!"<<endl;
                    //终止
                }
                if(currenttoken==")"){
                    if((currenttoken=get_token())=="{") {
                        IoStream();
                    }
                    else
                        cout << "Wrong function!" << endl;
                    //终止
                }
                else
                    cout<<"Wrong function!"<<endl;
                //终止
        }
            else
                cout<<"Wrong function!"<<endl;
        //终止
    }
    else
        cout<<"Wrong function!"<<endl;
    //终止
}
void IoStream(){
    currenttoken=get_token();
    if(currenttoken=="类型"){
        VariableSetting();
        currenttoken=get_token();
    }
    else if((VariableOutcome=VariableName())==1) {
        currenttoken = get_token();
        if (currenttoken == "(") {
            if (currenttoken == ")") {//无参数
                if ((currenttoken = get_token()) != ";") {
                    cout << "Wrong function!" << endl;
                    //终止
                }
                else
                    currenttoken=get_token();
            }
            else if ((VariableOutcome = IfType()) == 1) {//有参数
                currenttoken = get_token();
                if ((VariableOutcome = VariableName()) == 1) {
                    while ((currenttoken = get_token()) == ",") {
                        currenttoken = get_token();
                        if ((VariableOutcome = IfType()) == 1) {
                            currenttoken = get_token();
                            if ((VariableOutcome = VariableName()) != 1) {
                                cout << "Wrong function!" << endl;
                                //终止
                            }
                        } else
                            cout << "Wrong function!" << endl;
                        //终止
                    }
                    if(currenttoken==")"){
                        if((currenttoken = get_token()) != ";") {
                            cout << "Wrong function!" << endl;
                            //终止
                        }
                        else currenttoken=get_token();
                    }
                    else
                        cout << "Wrong function!" << endl;
                    //终止
                }
                else
                    cout << "Wrong function!" << endl;
                //终止
                }
            else
                cout << "Wrong function!" << endl;
            //终止
            }
        while(currenttoken==","){//赋值
            currenttoken=get_token();
            if((VariableOutcome = VariableName()) != 1){
                cout << "Wrong function!" << endl;
                //终止
            }
            currenttoken=get_token();
        }
        if(currenttoken=="+"){
            if((currenttoken=get_token())!="="){
                cout << "Wrong function!" << endl;
                //终止
            }
            else Expression();
        }
        else if(currenttoken=="-"){
            if((currenttoken=get_token())!="="){
                cout << "Wrong function!" << endl;
                //终止
            }
            else Expression();
        }
        else if(currenttoken=="="){
            Expression();
        }
        else
            cout << "Wrong function!" << endl;
        //终止
            if ((currenttoken = get_token()) != ";") {
                cout << "Wrong function!" << endl;
                //终止
            }
            else currenttoken=get_token();
        }
    else if(currenttoken=="break"){
        if((currenttoken=get_token())!=";"){
            cout << "Wrong function!" << endl;
            //终止
        }
        else currenttoken=get_token();
    }
    else if(currenttoken=="continue"){
        if((currenttoken=get_token())!=";"){
            cout << "Wrong function!" << endl;
            //终止
        }
        else currenttoken=get_token();
    }
    else if(currenttoken=="return"){
        Expression();
        if((currenttoken=get_token())!=";"){
            cout << "Wrong function!" << endl;
            //终止
        }
        else currenttoken=get_token();
    }
    else if(currenttoken=="if"){
        IfExpression();
    }
    else if(currenttoken=="while"){
        WhileExpression();
    }
    else
        cout << "Wrong function!" << endl;
        //终止
    }
void Expression(){
    currenttoken=get_token();
}
void IfExpression() {
    if ((currenttoken = get_token()) == "(") {
        Expression();
        if ((currenttoken = get_token()) == ")") {
            if ((currenttoken = get_token()) == "{") {
                IoStream();
                if (currenttoken == "}") {
                    if ((currenttoken = get_token()) == "else") {
                        if ((currenttoken = get_token()) == "if") {
                            IfExpression();
                        } else if ((currenttoken = get_token()) == "{") {
                            IoStream();
                            if(currenttoken!="}"){
                                cout << "Wrong If function!" << endl;
                                //终止
                            }
                        }
                    }
                }
                else
                    cout << "Wrong If function!" << endl;
                //终止
            }
            else
                cout << "Wrong If function!" << endl;
            //终止
        }
        else
            cout << "Wrong If function!" << endl;
        //终止
    }
    else
        cout << "Wrong If function!" << endl;
    //终止
}
void WhileExpression(){
    if((currenttoken = get_token()) == "("){
        Expression();
        if((currenttoken = get_token()) == ")"){
            if((currenttoken = get_token()) == "{"){
                IoStream();
                if(currenttoken!="}"){
                    cout << "Wrong While function!" << endl;
                    //终止
                }
            }
            cout << "Wrong While function!" << endl;
            //终止
        }
        cout << "Wrong While function!" << endl;
        //终止
    }
    cout << "Wrong While function!" << endl;
    //终止
}
