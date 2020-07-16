#include <iostream>
#include<vector>
#include<stack>
#include "semantic.h"
#include"gen.h"

using namespace std;



/********************************************/
//��������.cppӦͨ��
//�����Ϳ��Դﵽ�����.cpp�и�������������ʹ�ø���Ԫʽ
vector<STY> FourStyle;
vector<SymbolNode> Symbol; //һ���������⣬Symbol�����.cpp����������һ��������siyuanshi.cpp��û������
vector<block_begin_end> block;

/********************************************/

void getBlock(); // �����黮�֣�����block��
void ActiveInfo(int i); //��Ծ��Ϣ��д������FourStyle��
void InitSymbol(); //һ������ɺ󣬰�Symbol������

void getFourStyle() {   //��ȡ��Ԫʽ
    ifstream siyuanshi;
    siyuanshi.open("FourStyle1.txt");
    while (!siyuanshi.eof() && siyuanshi.peek() != EOF) {
        char aa, bb, cc, dd, ee;
        STY fs;
        siyuanshi >> aa >> fs.deli >> bb >> fs.first >> cc >> fs.second >> dd >> fs.third >> ee;
        fs.f = -1;
        fs.s = -1;
        fs.t = -1;      //��Ծ��Ϣȱʡֵ
        FourStyle.push_back(fs);
    }
    siyuanshi.close();
}

void getFourStyle(Quadruple *q) {

}


void getSymbol() {           //��ȡ������ű�
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
    for (m = 0; m < FourStyle.size() - 1; m++)           //.size()��ȡ�������Ĵ�С
    {
        cout << "(" << FourStyle[m].deli << "," << FourStyle[m].first << "," << FourStyle[m].second << ","
             << FourStyle[m].third /*<< "," << FourStyle[m].t */<< ")" << endl;
    }
    cout << m << endl;
}

void OUTPUTStoreSymbolNode() {
    int m;
    for (m = 0; m < Symbol.size() - 1; m++)           //.size()��ȡ�������Ĵ�С
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
    vector<string> OBJ;//���Ŀ�����
    stack<string> SEM;
    //vector<string> iden; //��ҪԤ�Ƚ��з����ڴ�ı�־��
    string s1; //������ŵ��µ�Ŀ�����
    string s2; //������ŵ��µ�Ŀ�����
    string s3;
    string ss;//������ʾ+-*/��Ӧ��Ŀ�����
    string su;
    RDL R; //ģ�ⵥ�Ĵ���
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
    //���п黮��
    getBlock();

    //�Ĵ�����ʼ��
    R.data = "_";
    R.state = 0;


    for (i = 0; i < block.size(); i++) {
        ActiveInfo(i);
        for (j = block[i].begin; j <= block[i].end; j++) {
            if (FourStyle[j].third != "_" && FourStyle[j].t == 1) {
                su = FourStyle[j].third;
                //iden.push_back(su); //�õ�Ҫ���д洢������Ҫ���ٿռ��һϵ������
                //0-��ʱ������1-����ʱ����
            }
        }
    }


    //���л�����Ե�Ԥ��д��
    //��ȡ��д��һ��vector OBJ�У����֮�������ļ���д
    //���ݶ�
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
    //��ջ��
    s1 = "SSEG   SEGMENT   STACK";
    OBJ.push_back(s1);
    s1 = "STK   DB   80H DUP(0)";
    OBJ.push_back(s1);
    s1 = "SSEG   ENDS";
    OBJ.push_back(s1);
    //�����
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
        //ȡ����л�Ծ��Ϣ�ı��
        //����Ծ��Ϣ����FourStyle��f,s,t,��
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
        //����Ŀ������д
        for (j = block[i].begin; j <= block[i].end; j++) {

            //�Ӽ��˳�
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
                        //��Ծ
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
                        } else  //����Ծ
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
                        if (FourStyle[j].s == 1) //��Ծ
                        {
                            s1 = "MOV " + FourStyle[j].second + ",AX";
                            OBJ.push_back(s1);
                        }
                        //���ܻ��Ծ
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
            //��ֵ

            //�߼�����
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
                        //��Ծ
                        if (FourStyle[j].f == 1) {
                            s1 = "MOV " + FourStyle[j].first + ",AX";
                            s2 = "MOV AX,AX " + ss + " " + FourStyle[j].second;
                            OBJ.push_back(s1);
                            OBJ.push_back(s2);
                        } else  //����Ծ
                        {
                            s1 = "MOV AX,AX " + ss + " " + FourStyle[j].second;
                            OBJ.push_back(s1);
                        }
                    } else if (R.data == FourStyle[j].second && (FourStyle[j].deli == "!=" || FourStyle[j].deli == "=="
                                                                 || FourStyle[j].deli == "&&" ||
                                                                 FourStyle[j].deli == "||")) {
                        if (FourStyle[j].s == 1) //��Ծ
                        {
                            s1 = "MOV " + FourStyle[j].second + ",AX";
                            OBJ.push_back(s1);
                        }
                        //���ܻ��Ծ
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

            //!����
            if (FourStyle[j].deli == "!") {
                if (R.data == "_") {
                    s1 = "MOV AX,NOT" + FourStyle[j].first;
                    OBJ.push_back(s1);
                } else {
                    if (R.data == FourStyle[j].first) {
                        //��Ծ
                        if (FourStyle[j].f == 1) {
                            s1 = "MOV " + FourStyle[j].first + ",AX";
                            s2 = "MOV AX,NOT AX";
                            OBJ.push_back(s1);
                            OBJ.push_back(s2);
                        } else  //����Ծ
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
                    //��ʱ��Ӧ�ÿ��Ĵ����д�����ݵ�state�����ǿ�FourStyle�еģ�
                    //��ΪFourStyle�������µ�
                    if (FourStyle[j].f == 1) {
                        s1 = "MOV " + FourStyle[j].first + ",AX";
                        OBJ.push_back(s1);
                    }
                }
                    //����Ϊ��ʱ����first�����ǿ�����third����ôֻ��ҪLD�Ϳ�����
                else if (R.data != FourStyle[j].first) {
                    //����third���һ�Ծ��ʱ�����ҪST��������ֻ��ҪLD
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
                //��ֹ���ʱ��Ĵ�����������
                //��Ҫ����ÿ������󶼻�ѼĴ����е����ݸ�Ϊ�ôε�third��������ݣ�
                //������wh����Ĵ������ܻ��ж�������
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
                s2 = "JBE   OUT" + to_string(a);//��������������ʱ������ȥ
                OBJ.push_back(s1);
                OBJ.push_back(s2);
                s3 = "OUT" + to_string(a);  //����ѹջ
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

            //����
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

            //ÿһ��������Ĵ����ж�����һ������
            //����ÿ����������Ϊÿ�ζ�һ�����ӽ��б�дĿ������ʱ�򣬸տ�ʼ��ʱ��Ĵ����еĶ�������һ�����ӵ�third
            //����Ҫ���if���޸ģ���Ϊif�е�third��\0��������ActiveInfoʱ����û���޸ģ���t����0
            R.data = FourStyle[j].third;
            R.state = FourStyle[j].t;
        }
        //��һ������ɺ�Ҫ��Symbol���������ù顰�㡱
        InitSymbol();
    }

    //��β����

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
    //�����Ϣ��
    for (i = 0; i < OBJ.size(); i++) {
        cout << OBJ[i] << '\n';
        Targetcode << OBJ[i] << '\n';
    }

}

void getBlock() {
    int i;  //��Ϊvector�� .size()�ķ���ֵ���޷�����
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
    //Ϊ�˷�ֹ������Ԫʽ��һϵ�еĸ�ֵ���
    //���ֻ�������Ŀ������ǣ��ᵼ�¶����ܶ����
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
    //����飬��Ծ��Ϣ������������
    for (j = block[i].end; j >= block[i].begin; j--)//ֻ�Ǳ�־����Ԫʽ�����
    {
        first = -1;
        second = -1;
        third = -1;
        //��Symbol���ҵ���Ӧ������
        for (m = 0; m < Symbol.size(); m++)  //Symbol���ݳ������⣬�ظ���һ��
        {
            if (FourStyle[j].first == Symbol[m].data) first = m;
            if (FourStyle[j].second == Symbol[m].data) second = m;
            if (FourStyle[j].third == Symbol[m].data) third = m;
            //Ҫע�⵽����Ԫʽ�ĺ�����Ԫ�ز���һ�����Ƿ��ţ������ǳ������ַ��������߿գ�����һ�����Ƿ��ŵģ�
            //���ԣ���ѭ��������ʱ��first,second,third�����е���-1��
        }
        //�Ը���Ԫʽ��ǻ�Ծ��Ϣ
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
        if (Symbol[i].type == 1) Symbol[i].state = 1; //����ʱ����������ʱ���Ϊy
        else Symbol[i].state = 0;//��ʱ������ֵΪn
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
