#include <iostream>
#include<vector>
#include<stack>
#include "semantic.h"
#include"gen.h"

using namespace std;



/********************************************/
//这三个本.cpp应通用
//这样就可以达到在这个.cpp中各个函数中随意使用该四元式
vector<STY> FourStyle;
vector<SymbolNode> Symbol; //一个严重问题，Symbol到这个.cpp后，数量多了一倍，但在siyuanshi.cpp中没有问题
vector<block_begin_end> block;

/********************************************/

void getBlock(); // 基本块划分；存在block中
void ActiveInfo(int i); //活跃信息填写；存在FourStyle中
void InitSymbol(); //一个块完成后，把Symbol表重置

void getFourStyle() {   //获取四元式
    ifstream siyuanshi;
    siyuanshi.open("FourStyle1.txt");
    while (!siyuanshi.eof() && siyuanshi.peek() != EOF) {
        char aa, bb, cc, dd, ee;
        STY fs;
        siyuanshi >> aa >> fs.deli >> bb >> fs.first >> cc >> fs.second >> dd >> fs.third >> ee;
        fs.f = -1;
        fs.s = -1;
        fs.t = -1;      //活跃信息缺省值
        FourStyle.push_back(fs);
    }
    siyuanshi.close();
}

void getFourStyle(Quadruple *q) {

}


void getSymbol() {           //获取建议符号表
    ifstream fuhaobiao;
    fuhaobiao.open("Symbol1.txt");
    while (!fuhaobiao.eof() && fuhaobiao.peek() != EOF) {
        char aa, bb, cc, dd;
        SymbolNode fhb;
        fuhaobiao >> aa >> fhb.data >> bb >> fhb.type >> cc >> fhb.state >> dd;
        Symbol.push_back(fhb);
    }
    fuhaobiao.close();
}

void OUTPUT() {
    int m;
    for (m = 0; m < FourStyle.size() - 1; m++)           //.size()获取行向量的大小
    {
        cout << "(" << FourStyle[m].deli << "," << FourStyle[m].first << "," << FourStyle[m].second << ","
             << FourStyle[m].third /*<< "," << FourStyle[m].t */<< ")" << endl;
    }
    cout << m << endl;
}

void OUTPUTStoreSymbolNode() {
    int m;
    for (m = 0; m < Symbol.size() - 1; m++)           //.size()获取行向量的大小
    {
        cout << "(" << Symbol[m].data << "," << Symbol[m].type << "," << Symbol[m].state << ")" << endl;
    }
    cout << m << endl;
}


void TargetCode(const vector<string> &iden, const string &outputpath) {
    int i;
    int j;
    //unsigned int locate;
    //unsigned int locate_1;
    vector<string> OBJ;//存放目标代码
    stack<string> SEM;
    //vector<string> iden; //需要预先进行分配内存的标志符
    string s1; //用来存放当下的目标代码
    string s2; //用来存放当下的目标代码
    string s3;
    string ss;//用来表示+-*/对应的目标代码
    string su;
    RDL R; //模拟单寄存器
    /*
    ifstream fuhaobiao;
    fuhaobiao.open("Symbol1.txt");
    while (!fuhaobiao.eof() && fuhaobiao.peek() != EOF) {
        char aa, bb, cc, dd;
        SymbolNode fhb;
        fuhaobiao >> aa >> fhb.data >> bb >> fhb.type >> cc >> fhb.state >> dd;
        Symbol.push_back(fhb);
    }
     */
    int a = 0;
    //FourStyle = siyuanshi();
    //进行块划分
    getBlock();

    //寄存器初始化
    R.data = "_";
    R.state = 0;


    for (i = 0; i < block.size(); i++) {
        ActiveInfo(i);
        for (j = block[i].begin; j <= block[i].end; j++) {
            if (FourStyle[j].third != "_" && FourStyle[j].t == 1) {
                su = FourStyle[j].third;
                //iden.push_back(su); //得到要进行存储，并需要开辟空间的一系列数据
                //0-临时变量；1-非临时变量
            }
        }
    }


    //进行汇编语言的预编写；
    //采取先写到一个vector OBJ中，完成之后再向文件中写
    //数据端
    s1 = "DSEG   SEGMENT";
    OBJ.push_back(s1);
    //
    for (i = 0; i < iden.size() - 1; i++) {
        s1 = iden[i] + "   " + "Dw" + "   " + "?";
        OBJ.push_back(s1);
    }
    //cout << "DSduan mei wen ti" << endl;
    s1 = "DSEG   ENDS";
    OBJ.push_back(s1);
    //堆栈段
    s1 = "SSEG   SEGMENT   STACK";
    OBJ.push_back(s1);
    s1 = "STK   DB   80H DUP(0)";
    OBJ.push_back(s1);
    s1 = "SSEG   ENDS";
    OBJ.push_back(s1);
    //代码段
    s1 = "CSEG   SEGMENT";
    OBJ.push_back(s1);
    s1 = "      ASSUME   CS:CSEG,DS:DSEG,SS:SSEG";
    OBJ.push_back(s1);
    s1 = "START:   MOV AX,DSEG";
    OBJ.push_back(s1);
    s1 = "         MOV DS,AX";
    OBJ.push_back(s1);
    s1 = "         MOV AX,SSEG";
    OBJ.push_back(s1);
    s1 = "         MOV SS,AX";
    OBJ.push_back(s1);
    s1 = "         MOV SP,SIZE STK";
    OBJ.push_back(s1);
    s1 = "         MOV AX,0000H";
    OBJ.push_back(s1);


    for (i = 0; i < block.size(); i++) {
        //取块进行活跃信息的标记
        //最后活跃信息放在FourStyle的f,s,t,中
        ActiveInfo(i);
        //********************

        for (j = block[i].begin; j < block[i].end; j++) {
            cout << FourStyle[j].deli << "  ";
            cout << FourStyle[j].first << "(" << FourStyle[j].f << ")" << "   ";
            cout << FourStyle[j].second << "(" << FourStyle[j].s << ")" << "   ";
            cout << FourStyle[j].third << "(" << FourStyle[j].t << ")" << "   ";
            cout << endl;
        }
        //********************
        //进行目标代码编写
        for (j = block[i].begin; j <= block[i].end; j++) {

            //加减乘除
            if (FourStyle[j].deli == "+" || FourStyle[j].deli == "-"
                || FourStyle[j].deli == "*" || FourStyle[j].deli == "/") {
                if (FourStyle[j].deli == "+") ss = "ADD ";
                if (FourStyle[j].deli == "-") ss = "SUB ";
                if (FourStyle[j].deli == "*") ss = "MUL ";
                if (FourStyle[j].deli == "/") ss = "DIV ";

                if (R.data == "_") {
                    s1 = "MOV AX," + FourStyle[j].first;
                    if (FourStyle[j].deli != "*" && FourStyle[j].deli != "/") {
                        s2 = ss + " AX," + FourStyle[j].second;
                        OBJ.push_back(s1);
                        OBJ.push_back(s2);
                    } else {
                        s3 = "MOV BX," + FourStyle[j].second;
                        s2 = ss + "BX";
                        OBJ.push_back(s1);
                        OBJ.push_back(s2);
                        OBJ.push_back(s3);
                    }
                } else {
                    if (R.data == FourStyle[j].first) {
                        //活跃
                        if (FourStyle[j].f == 1) {
                            s1 = "MOV " + FourStyle[j].first + ",AX";
                            //s2 = ss + " AX," + FourStyle[j].second;
                            if (FourStyle[j].deli != "*" && FourStyle[j].deli != "/") {
                                s2 = ss + " AX," + FourStyle[j].second;
                            } else {
                                s3 = "MOV BX," + FourStyle[j].second;
                                s2 = ss + "BX";
                            }
                            OBJ.push_back(s1);
                            OBJ.push_back(s3);
                            OBJ.push_back(s2);
                        } else  //不活跃
                        {
                            if (FourStyle[j].deli != "*" && FourStyle[j].deli != "/") {
                                s2 = ss + " AX," + FourStyle[j].second;
                                OBJ.push_back(s2);
                            } else {
                                s3 = "MOV BX," + FourStyle[j].second;
                                s2 = ss + "BX";
                                OBJ.push_back(s3);
                                OBJ.push_back(s2);
                            }
                        }
                    } else if (R.data == FourStyle[j].second &&
                               (FourStyle[j].deli == "+" || FourStyle[j].deli == "*")) {
                        if (FourStyle[j].s == 1) //活跃
                        {
                            s1 = "MOV " + FourStyle[j].second + ",AX";
                            OBJ.push_back(s1);
                        }
                        //不管活不活跃
                        //s2 = ss + " AX," + FourStyle[j].first;
                        if (FourStyle[j].deli != "*" && FourStyle[j].deli != "/") {
                            s2 = ss + " AX," + FourStyle[j].second;
                            OBJ.push_back(s2);
                        } else {
                            s3 = "MOV BX," + FourStyle[j].second;
                            s2 = ss + "BX";
                            OBJ.push_back(s3);
                            OBJ.push_back(s2);
                        }
                    } else {
                        if (R.state == 1) {
                            s1 = "MOV " + R.data + ",AX";
                            OBJ.push_back(s1);
                        }
                        s2 = "MOV AX," + FourStyle[j].first;
                        //s3 = ss + " AX," + FourStyle[j].second;
                        if (FourStyle[j].deli != "*" && FourStyle[j].deli != "/") {
                            s2 = ss + " AX," + FourStyle[j].second;
                            OBJ.push_back(s2);
                        } else {
                            s3 = "MOV BX," + FourStyle[j].second;
                            s2 = ss + "BX";
                            OBJ.push_back(s3);
                            OBJ.push_back(s2);
                        }
                    }
                }
            }
            //赋值

            //逻辑运算
            if (FourStyle[j].deli == ">" || FourStyle[j].deli == "<"
                || FourStyle[j].deli == ">=" || FourStyle[j].deli == "<="
                || FourStyle[j].deli == "==" || FourStyle[j].deli == "!="
                || FourStyle[j].deli == "&&" || FourStyle[j].deli == "||") {
                if (FourStyle[j].deli == ">") ss = "GT";
                if (FourStyle[j].deli == "<") ss = "LT";
                if (FourStyle[j].deli == ">=") ss = "GE";
                if (FourStyle[j].deli == "<=") ss = "LE";
                if (FourStyle[j].deli == "==") ss = "EQ";
                if (FourStyle[j].deli == "!=") ss = "NE";
                if (FourStyle[j].deli == "&&") ss = "AND";
                if (FourStyle[j].deli == "||") ss = "OR";

                if (R.data == "_") {
                    s1 = "MOV AX," + FourStyle[j].first;
                    s2 = "MOV AX,AX " + ss + " " + FourStyle[j].second;
                    OBJ.push_back(s1);
                } else {
                    if (R.data == FourStyle[j].first) {
                        //活跃
                        if (FourStyle[j].f == 1) {
                            s1 = "MOV " + FourStyle[j].first + ",AX";
                            s2 = "MOV AX,AX " + ss + " " + FourStyle[j].second;
                            OBJ.push_back(s1);
                            OBJ.push_back(s2);
                        } else  //不活跃
                        {
                            s1 = "MOV AX,AX " + ss + " " + FourStyle[j].second;
                            OBJ.push_back(s1);
                        }
                    } else if (R.data == FourStyle[j].second && (FourStyle[j].deli == "!=" || FourStyle[j].deli == "=="
                                                                 || FourStyle[j].deli == "&&" ||
                                                                 FourStyle[j].deli == "||")) {
                        if (FourStyle[j].s == 1) //活跃
                        {
                            s1 = "MOV " + FourStyle[j].second + ",AX";
                            OBJ.push_back(s1);
                        }
                        //不管活不活跃
                        s2 = "MOV AX,AX " + ss + " " + FourStyle[j].first;
                        OBJ.push_back(s2);
                    } else {
                        if (R.state == 1) {
                            s1 = "MOV " + R.data + ",AX";
                            OBJ.push_back(s1);
                        }
                        s2 = "MOV AX," + FourStyle[j].first;
                        s3 = "MOV AX,AX " + ss + " " + FourStyle[j].second;
                        OBJ.push_back(s2);
                        OBJ.push_back(s3);
                    }
                }
            }

            //!运算
            if (FourStyle[j].deli == "!") {
                if (R.data == "_") {
                    s1 = "MOV AX,NOT" + FourStyle[j].first;
                    OBJ.push_back(s1);
                } else {
                    if (R.data == FourStyle[j].first) {
                        //活跃
                        if (FourStyle[j].f == 1) {
                            s1 = "MOV " + FourStyle[j].first + ",AX";
                            s2 = "MOV AX,NOT AX";
                            OBJ.push_back(s1);
                            OBJ.push_back(s2);
                        } else  //不活跃
                        {
                            s1 = "MOV AX,NOT AX";
                            OBJ.push_back(s1);
                        }
                    } else {
                        if (R.state == 1) {
                            s1 = "MOV " + R.data + ",AX";
                            OBJ.push_back(s1);
                        }
                        s2 = "MOV AX,NOT" + FourStyle[j].first;
                        OBJ.push_back(s2);
                    }
                }
            }

            if (FourStyle[j].deli == "=") {
                if (R.data == "_") {
                    s1 = "MOV AX," + FourStyle[j].first;
                    OBJ.push_back(s1);
                } else if (R.data == FourStyle[j].first) {
                    //这时候不应该看寄存器中存的数据的state，而是看FourStyle中的；
                    //因为FourStyle中是最新的
                    if (FourStyle[j].f == 1) {
                        s1 = "MOV " + FourStyle[j].first + ",AX";
                        OBJ.push_back(s1);
                    }
                }
                    //我认为这时候不是first，但是可以是third，那么只需要LD就可以了
                else if (R.data != FourStyle[j].first) {
                    //不是third并且活跃的时候才需要ST，其他的只需要LD
                    if (R.state == 1 && R.data != FourStyle[j].third) {
                        s1 = "MOV " + R.data + ",AX";
                        OBJ.push_back(s1);
                    }
                    s2 = "MOV AX," + FourStyle[j].first;
                    OBJ.push_back(s2);
                }
            }

            //if
            if (FourStyle[j].deli == "if") {
                if (R.data == "_") {
                    s1 = "MOV AX," + FourStyle[j].first;
                    OBJ.push_back(s1);
                } else if (R.data == FourStyle[j].first) {
                    if (FourStyle[j].f == 1) {
                        s1 = "MOV " + FourStyle[j].first + ",AX";
                        OBJ.push_back(s1);
                    }
                } else {
                    if (R.state == 1) {
                        s1 = "MOV " + R.data + ",AX";
                        OBJ.push_back(s1);
                    }
                    s2 = "MOV AX," + FourStyle[j].first;
                    OBJ.push_back(s2);
                }
                s1 = "IFF" + to_string(a) + ":";
                OBJ.push_back(s1);
                s2 = "CMP AL,0000H";
                OBJ.push_back(s2);
                s3 = "JE ELSE" + to_string(a);
                OBJ.push_back(s3);
                SEM.push("ENDIF" + to_string(a) + ":");
                SEM.push("ELSE" + to_string(a) + ":");
            }

            if (FourStyle[j].deli == "el") {
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",AX";
                    OBJ.push_back(s1);
                }
                OBJ.push_back(SEM.top());
                SEM.pop();
            }

            if (FourStyle[j].deli == "ie") {
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",AX";
                    OBJ.push_back(s1);
                }
                OBJ.push_back(SEM.top());
                SEM.pop();
            }


            if (FourStyle[j].deli == "wh") {
                //防止这个时候寄存器中有数据
                //主要是我每次在最后都会把寄存器中的数据该为该次的third的相关数据；
                //所以在wh这里寄存器可能会有东西存在
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",R";
                    OBJ.push_back(s1);
                }
            }

            if (FourStyle[j].deli == "do") {
                if (R.data == "_") {
                    s1 = "MOV AX," + FourStyle[j].first;
                    OBJ.push_back(s1);
                } else if (R.data == FourStyle[j].first) {
                    if (FourStyle[j].f == 1) {
                        s1 = "MOV " + FourStyle[j].first + ",AX";
                        OBJ.push_back(s1);
                    }
                } else {
                    if (R.state == 1) {
                        s1 = "MOV " + R.data + ",AX";
                        OBJ.push_back(s1);
                    }
                    s1 = "MOV AX," + FourStyle[j].first;
                }
                s1 = "LOOP" + to_string(a) + ": " + "CMP   AX,0000H";
                s2 = "JBE   OUT" + to_string(a);//当不满足条件的时候，跳出去
                OBJ.push_back(s1);
                OBJ.push_back(s2);
                s3 = "OUT" + to_string(a);  //逆序压栈
                SEM.push(s3);
                s3 = "LOOP" + to_string(a);
                SEM.push(s3);
                a++;
            }

            if (FourStyle[j].deli == "we") {
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",AX";
                    OBJ.push_back(s1);
                }
                s3 = SEM.top();
                s1 = "JMP   " + s3;
                OBJ.push_back(s1);
                SEM.pop();

                s3 = SEM.top();
                s1 = s3 + ":";
                OBJ.push_back(s1);
                SEM.pop();

            }

            if (FourStyle[j].deli == "print") {
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",AX";
                    OBJ.push_back(s1);
                }
                s1 = "LEA DX," + FourStyle[j].third;
                s2 = "MOV AH,09H";
                s3 = "INT 21H";
                OBJ.push_back(s1);
                OBJ.push_back(s2);
                OBJ.push_back(s3);
            }

            //函数
            if (FourStyle[j].deli == "func_de") {
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",AX";
                    OBJ.push_back(s1);
                }
                s1 = FourStyle[j].third + "  PROC   NEAR";
                OBJ.push_back(s1);
                SEM.push(FourStyle[j].third);
            }

            if (FourStyle[j].deli == "func_end") {
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",AX";
                    OBJ.push_back(s1);
                }
                s1 = "RET";
                OBJ.push_back(s1);
                s2 = SEM.top();
                s1 = s2 + "   ENDP";
                SEM.pop();
                OBJ.push_back(s1);
            }

            if (FourStyle[j].deli == "func_use") {
                if (R.data != "_" && R.state == 1) {
                    s1 = "MOV " + R.data + ",AX";
                    OBJ.push_back(s1);
                }
                s1 = "CALL   " + FourStyle[j].third;
                OBJ.push_back(s1);
            }

            //每一句结束，寄存器中都会有一个数据
            //放在每句的最后，是因为每次读一个句子进行编写目标代码的时候，刚开始的时候寄存器中的东西是上一个句子的third
            //不需要针对if做修改，因为if中的third是\0；所以在ActiveInfo时，并没有修改，其t还是0
            R.data = FourStyle[j].third;
            R.state = FourStyle[j].t;
        }
        //在一个块完成后，要把Symbol的数据重置归“零”
        InitSymbol();
    }

    //结尾处理

    s1 = "MOV   AH,4CH";
    s2 = "INT   21H";
    OBJ.push_back(s1);
    OBJ.push_back(s2);
    s1 = "CSEG   ENDS";
    s2 = "      END   START";
    OBJ.push_back(s1);
    OBJ.push_back(s2);

    fstream Targetcode(outputpath, ios::out);
    if (!Targetcode) {
        cerr << "Can't open path: " << outputpath;
    }
    //输出信息；
    for (i = 0; i < OBJ.size(); i++) {
        cout << OBJ[i] << '\n';
        Targetcode << OBJ[i] << '\n';
    }

}

void getBlock() {
    int i;  //因为vector中 .size()的返回值是无符号数
    int n;
    block_begin_end a;
    a.begin = 0;
    for (i = 0; i < FourStyle.size(); i++) {
        if (FourStyle[i].deli == "if") {
            a.end = i;
            block.push_back(a);
            a.begin = i + 1;
        }
        if (FourStyle[i].deli == "el") {
            a.end = i;
            block.push_back(a);
            a.begin = i + 1;
        }
        if (FourStyle[i].deli == "ie") {
            a.end = i;
            block.push_back(a);
            a.begin = i + 1;
        }
        if (FourStyle[i].deli == "wh") {
            a.end = i;
            block.push_back(a);
            a.begin = i + 1;
        }
        if (FourStyle[i].deli == "do") {
            a.end = i;
            block.push_back(a);
            a.begin = i + 1;
        }
        if (FourStyle[i].deli == "we") {
            a.end = i;
            block.push_back(a);
            a.begin = i + 1;
        }
    }
    n = FourStyle.size();
    //为了防止最后的四元式是一系列的赋值语句
    //如果只有上述的块结束标记，会导致丢掉很多语句
    if (FourStyle[n - 1].deli != "if" && FourStyle[n - 1].deli != "el" && FourStyle[n - 1].deli != "ie"
        && FourStyle[n - 1].deli != "wh" && FourStyle[n - 1].deli != "do" && FourStyle[n - 1].deli != "we") {
        a.end = n - 1;
        block.push_back(a);
    }
}

void ActiveInfo(int i) {
    int m;
    int first;
    int second;
    int third;
    int j;
    //这个块，活跃信息用逆序来设置
    for (j = block[i].end; j >= block[i].begin; j--)//只是标志了四元式的序号
    {
        first = -1;
        second = -1;
        third = -1;
        //在Symbol中找到对应的数据
        for (m = 0; m < Symbol.size(); m++)  //Symbol数据出现问题，重复了一倍
        {
            if (FourStyle[j].first == Symbol[m].data) first = m;
            if (FourStyle[j].second == Symbol[m].data) second = m;
            if (FourStyle[j].third == Symbol[m].data) third = m;
            //要注意到，四元式的后三个元素并不一定都是符号；可以是常数、字符串、或者空；并不一定都是符号的；
            //所以，到循环结束的时候first,second,third可能有的是-1；
        }
        //对该四元式标记活跃信息
        if (first != -1) {
            FourStyle[j].f = Symbol[first].state;
            Symbol[first].state = 1;
        }
        if (second != -1) {
            FourStyle[j].s = Symbol[second].state;
            Symbol[second].state = 1;
        }
        if (third != -1) {
            FourStyle[j].t = Symbol[third].state;
            Symbol[third].state = 0;
        }
    }
}

void InitSymbol() {
    unsigned int i;
    for (i = 0; i < Symbol.size() / 2; i++) {
        if (Symbol[i].type == 1) Symbol[i].state = 1; //非临时变量出来的时候变为y
        else Symbol[i].state = 0;//临时变量初值为n
    }
}
/*
int main()
{
    getFourStyle();
    getSymbol();
	OUTPUT();
    OUTPUTStoreSymbolNode();
	TargetCode();

	return 0;

}
 */
