#include <iostream>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "lex.h"

using namespace std;

static int i = 0, line_num = 1, colomn_num = 0;

static Token_type syn = TK_EOF;

const size_t TOKENSIZE = 180;
static char // resourceProject[100000],
token[TOKENSIZE] = {0}; //改成可以保留起来,get_token时复制一份里面内容

static ifstream resourceProject;

/* 已弃用
//词法分析程序
//首先定义种别码
对所有可数符号进行编码：
<$,          0>
<auto,       1>
<break,      2>
<bool,       3>
<case,       4>
<char,       5>
<const,      6>
<class,      7>
<continue,   8>
<cin,        9>
<cout,      10>
<default,   11>
<do,        12>
<double,    13>
<delete,    14>
<else,      15>
<enum,      16>
<extern,    17>
<float,     18>
<for,       19>
<friend,    20>
<fprintf,   21>
<fopen,     22>
<fclose,    23>
<goto,      24>
<if,        25>
<int,       26>
<inline,    27>
<long,      28>
<malloc,    29>
<main,      30>
<number,    31>
<new,       32>
<printf,    33>
<public,    34>
<private,   35>
<protected, 36>
<return,    37>
<short,     38>
<signed,    39>
<sizeof,    40>
<static,    41>
<struct,    42>
<switch,    43>
<typedef,   44>
<this,      45>
<union,     46>
<unsigned,  47>
<void,      48>
<while,     49>
<;，        50>
<(,         51>
<),         52>
<,,         53>
<#,         54>
<~,         55>
<[,         56>
<],         57>
<{,         58>
<},         59>
<.,         60>
<?,         61>
<:,         62>
<',         63>
<",         64>
<+,         65>
<++,        66>
<+=,        67>
<-,         68>
<--,        69>
<-=,        70>
<->,        71>
<*,         72>
<*=,        73>
</,         74>
</=,        75>
<%,         76>
<%=,        77>
<<<=,       78>
<<<,        79>
<<=,        80>
<<,         81>
<>>=,       82>
<>>,        83>
<>=,        84>
<>,         85>
<&,         86>
<&&,        87>
<&=,        88>
<|,         89>
<||,        90>
<|=,        91>
<^,         92>
<^=,        93>
<=,         94>
<==,        95>
<!,         96>
<!=,        97>
<\,         98>
<\n,        99>
<\t,        100>
<\v,        101>
<\r,        102>
<常数,      120>
<标识符,    121>
*/

/****************************************************************************************/
//全局变量，保留字表
static std::unordered_map<string, Token_type> reserveWord{
    {{"extern"}, TK_EXTERN},
        {{"typedef"}, TK_TYPEDEF},
        {{"number"}, TK_NUMBERTYPE},
        {{"char"}, TK_CHARTYPE},
        {{"string"}, TK_STRINGTYPE},
        {{"void"}, TK_VOID},
        {{"struct"}, TK_STRUCT},
        {{"bool"}, TK_BOOL},
        {{"if"}, TK_IF},
        {{"else"}, TK_ELSE},
        {{"for"}, TK_FOR},
        {{"do"}, TK_DO},
        {{"while"}, TK_WHILE},
        {{"switch"}, TK_SWITCH},
        {{"case"}, TK_CASE},
        {{"default"}, TK_DEFAULT},
        {{"break"}, TK_BREAK},
        {{"continue"}, TK_CONTINUE},
        {{"new"}, TK_NEW},
        {{"delete"}, TK_DELETE},
        {{"main"}, TK_MAIN},
        {{"return"}, TK_RETURN},
        {{"sizeof"}, TK_SIZEOF},
};
//界符运算符表,根据需要可以自行增加
/*
   static const char *operatorOrDelimiter[53] = {
   ";",  "(",  ")",  ",",  "#",  "~",  "[",   "]",  "{",  "}",  ".",
   "?",  ":",  "\'", "\"", "+",  "++", "+=",  "-",  "--", "-=", "->",
   "*",  "*=", "/",  "/=", "%",  "%=", "<<=", "<<", "<=", "<",  ">>=",
   ">>", ">=", ">",  "&",  "&&", "&=", "|",   "||", "|=", "^",  "^=",
   "=",  "==", "!",  "!=", "\\", "\n", "\t",  "\v", "\r"};
   */
/********查找保留字*****************/
Token_type searchReserve(char *token) {
    auto it = reserveWord.find(token);
    return (it == reserveWord.end()
            ? TK_IDENT
            : it->second); //否则返回-1，代表查找不成功，即为标识符
}
/********查找保留字*****************/

/*********************判断是否为字母********************/
bool IsLetter(char letter) { //注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if (isalpha(letter) || letter == '_') {
        return true;
    } else {
        return false;
    }
}
/*********************判断是否为字母********************/

/*****************判断是否为数字************************/
bool IsDigit(char digit) { return (digit >= '0' && digit <= '9'); }
/*****************判断是否为数字************************/

/*****取出转义字符ascii码**********/
int getEscapeChar(int ch) {
    switch (ch) {
        case '\\':
            return '\\';
            break;
        case '\'':
            return '\'';
            break;
        case '\"':
            return '"';
            break;
        case 'n':
            return '\n';
            break;
        case 't':
            return '\t';
            break;
        case 'r':
            return '\r';
            break;
        case 'v':
            return '\v';
            break;
        case 'a':
            return '\a';
            break;
        case 'b':
            return '\b';
            break;
        case 'f':
            return '\f';
            break;
        default:
            cout << "Escape character error";
            exit(1); // TODO: 报错
    }
}
/*****取出转义字符ascii码**********/

/********************编译预处理，取出无用的字符和注释**********************/

void filterResource() {
    char last_char;
    while (resourceProject >> last_char && last_char == '/') {
        if (resourceProject.peek() ==
                '/') { //若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (resourceProject >> last_char && last_char != '\n')
                ; //向后扫描
            line_num++;
            colomn_num = 0;
        } else if (resourceProject.peek() ==
                '*') { //若为多行注释“/* ... */”则去除该内容
            resourceProject.get();
            while (resourceProject >> last_char &&
                    (last_char != '*' || resourceProject.peek() != '/')) {
                if (last_char == '\n') {
                    line_num++;
                    colomn_num = 0;
                }
            }
            if (!resourceProject) {
                cerr << "Error: comment doesn't end"<<"\nline_num:"<<line_num;//报错
                exit(1);
            }
        } else {
            break;
        }
    }
    if (resourceProject) {
        resourceProject.unget();
    }
}

/********************编译预处理，取出无用的字符和注释**********************/

/****************************分析子程序，算法核心***********************/
/*
 * i放在上个token结束的下个位置
 */
void Scanner() { //根据DFA的状态转换图设计
    int count = 0; // count用来做token[]的指示器，收集有用字符
    char ch;       //作为判断使用

    memset(token, 0, sizeof(char) * TOKENSIZE);
    if (!resourceProject) {
        goto eof;
    }

loop:               // 删注释，删空白，计行号
    filterResource(); //删注释
    while (resourceProject >> ch &&
            isspace(ch)) { //过滤空格，防止程序因识别不了空格而结束
        if (ch == '\n') {
            line_num++;
            colomn_num = 0;
        }
    }
    if (!resourceProject) {
        goto eof;
    }
    if (ch == '/') {
        resourceProject.unget();
        goto loop;
    }
    // resourceProject[i] => ch
    // i++ => resourceProject>>ch;    if(!resourceProject){ goto eof;}

    // i-- => resourceProject.unget();
    if (IsLetter(ch)) {    //开头为字母
        token[count++] = ch; //收集
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        }; //下移
        // filterResource();
        while (IsLetter(ch) || //
                IsDigit(ch)) {  //后跟字母或数字
            token[count++] = ch; //收集
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            } //下移
            // filterResource();
        }    //多读了一个字符既是下次将要开始的指针位置
        token[count] = '\0';
        syn = searchReserve(token); //查表找到种别码

    } else if (IsDigit(ch)) { //首字符为数字
        while (IsDigit(ch)) {   //后跟数字//TODO
            token[count++] = ch;  //收集
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
            // filterResource();
        } //多读了一个字符既是下次将要开始的指针位置

        if (ch == '.') {
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
            while (IsDigit(ch)) {  //后跟数字//TODO
                token[count++] = ch; //收集
                resourceProject >> ch;
                if (!resourceProject) {
                    goto eof;
                };
                // filterResource();
            }
        }
        token[count] = '\0';
        syn = TK_NUM; //常数种别码
    } else if (ch == ';' || ch == '(' || ch == ')' || ch == ',' || ch == '#' ||
            ch == '~' || ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
            ch == '.' || ch == '?' ||
            ch == ':') { //若为运算符或者界符，查表得到结果
        token[0] = ch;
        token[1] = '\0'; //形成单字符串

        syn = static_cast<Token_type>(ch);
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        }; //指针下移，为下一扫描做准备

    } else if (ch == '\'') { // 字符
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        if (ch == '\\') {
            ++i;
            getEscapeChar(ch);
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
        } else {
            token[0] = ch;
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
        }
        //判断字符是否终止
        if (ch == '\'') {
            // ok
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
        } else {
            cerr << "Error: Character too long."<<"\nline_num:"<<line_num; //报错
            exit(1);
        }
        syn = TK_CHAR;
    } else if (ch == '\"') { // 字符串
read:
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        if (ch != '\\' && ch != '\"') {
            token[count++] = ch;
            goto read;
        } else if (ch != '\"') { //转义
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
            token[count++] = getEscapeChar(ch);
            goto read;
        } else { /* == " */
            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
        }
        syn = TK_STR;
    } else if (ch == '<') { //<,<=,<<,<<=
        token[count++] = '<';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        }; //后移，超前搜索
        // filterResource();
        if (ch == '<') { //左移
            token[count++] = '<';

            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
            // filterResource();
            if (ch == '=') {
                token[count++] = '=';

                syn = TK_SHL_EQ;
            } else {
                resourceProject.unget();
                ;
                syn = TK_SHL;
            }

        } else if (ch == '=') {
            token[count++] = '=';

            syn = TK_LE;
        } else {
            resourceProject.unget();
            ;
            syn = TK_LT;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        }; //指针下移

    } else if (ch == '>') { //>,>=,>>，>>=
        token[count++] = '>';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '>') {
            token[count++] = '>';

            resourceProject >> ch;
            if (!resourceProject) {
                goto eof;
            };
            // filterResource();
            if (ch == '=') {
                token[count++] = '=';

                syn = TK_SHR_EQ;
            } else {
                resourceProject.unget();
                ;
                syn = TK_SHR;
            }
        } else if (ch == '=') {
            token[count++] = '=';

            syn = TK_GE;
        } else {
            resourceProject.unget();
            ;
            syn = TK_GT;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '=') { //=.==
        token[count++] = '=';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '=') {
            token[count++] = '=';

            syn = TK_EQ;
        } else {
            resourceProject.unget();
            ;
            syn = TK_ASSIGN;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '!') { //!,!=
        token[count++] = '!';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '=') {
            token[count++] = '=';

            syn = TK_NE;
        } else {
            syn = TK_EX;
            resourceProject.unget();
            ;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '&') { //&,&&,&=
        token[count++] = '&';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '&') {
            token[count++] = '&';

            syn = TK_LOGAND;
        } else if (ch == '=') {
            token[count++] = '=';

            syn = TK_AND_EQ;
        } else {
            resourceProject.unget();
            ;
            syn = TK_AND;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '|') { //|,||,|=
        token[count++] = '|';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '|') {
            token[count++] = '|';

            syn = TK_LOGOR;
        } else if (ch == '=') {
            token[count++] = '=';

            syn = TK_OR_EQ;
        } else {
            resourceProject.unget();
            ;
            syn = TK_OR;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '+') { //+,++,+=
        token[count++] = '+';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '+') {
            token[count++] = '+';

            syn = TK_INC;
        } else if (ch == '=') {
            token[count++] = '=';

            syn = TK_ADD_EQ;
        } else {
            resourceProject.unget();
            ;
            syn = TK_ADD;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '-') { //-,--,-=,->
        token[count++] = '-';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '-') {
            token[count++] = '-';

            syn = TK_DEC;
        } else if (ch == '=') {
            token[count++] = '=';

            syn = TK_SUB_EQ;
        } else if (ch == '>') {
            token[count++] = '>';

            syn = TK_ARROW;
        } else {
            resourceProject.unget();
            ;
            syn = TK_SUB;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '*') { //*,*=
        token[count++] = '*';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '=') {
            token[count++] = '=';

            syn = TK_MUL_EQ;
        } else {
            resourceProject.unget();
            ;
            syn = TK_MUL;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '/') { ///,/=
        token[count++] = '/';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '=') {
            token[count++] = '=';

            syn = TK_DIV_EQ;
        } else {
            resourceProject.unget();
            ;
            syn = TK_DIV;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '%') { //%,%=
        token[count++] = '%';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '=') {
            token[count++] = '=';

            syn = TK_MOD_EQ;
        } else {
            resourceProject.unget();
            ;
            syn = TK_MOD;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (ch == '^') { //^,^=
        token[count++] = '^';

        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };
        // filterResource();
        if (ch == '=') {
            token[count++] = '=';

            syn = TK_XOR_EQ;
        } else {
            resourceProject.unget();
            syn = TK_XOR;
        }
        resourceProject >> ch;
        if (!resourceProject) {
            goto eof;
        };

    } else if (!resourceProject) { //结束符
eof:
        syn = TK_EOF; //种别码为0
        token[0] = 'E';
        token[1] = 'O';
        token[2] = 'F';
        return;
    } else { //不能被以上词法分析识别，则出错。
        printf("error：there is no exist %c \n", ch);
        exit(1);
    }
    resourceProject.unget();
}
/*
 * Token get_token();
 */

Token get_token() {
    Scanner();
    ++colomn_num;
    // output
    int len = strlen(token);
    char *name = new char[len + 1];
    strcpy(name, token);
    return {name, syn, line_num, colomn_num};
}

void Init_lexer(const char *path) {
    if (resourceProject.is_open())
        resourceProject.close();
    resourceProject = ifstream(path, ios::in);
    if (!resourceProject) { //打开源程序
        cout << "can't open this file";
        exit(0);
    }
    resourceProject.unsetf(std::ios_base::skipws);
}
//依次读入文件，遇到注释就全部跳过

int main() {
    Init_lexer("testfile.txt");
    auto i = get_token();
    while (i.token_type != TK_EOF) {
        cout << i.Name << ' ' << i.token_type << ' ' << i.line_num << ' '
            << i.colomn_num << endl;
        i = get_token();
        getchar();
    }
}
