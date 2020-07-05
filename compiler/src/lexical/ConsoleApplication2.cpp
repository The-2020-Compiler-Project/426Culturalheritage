 #include "stdio.h"
 #include "stdlib.h"
 #include "string.h"
 #include "iostream"
 #include "conio.h"
 #include "iomanip"
 using namespace std;
 int i = 0, line_num = 1, colomn_num = 0, syn = -1,pProject = 0; //源程序指针
 char resourceProject[10000] ,token[20] = { 0 };
 
 //词法分析程序
 //首先定义种别码
 /*
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
 static char reserveWord[49][20] = {
     "auto", "break","bool", "case", "char", "const","class", "continue",
	 "cin","cout" "default", "do", "double","delete", "else", "enum", "extern",
     "float", "for","friend","fprintf","fopen","fclose", "goto", "if", "int",
	 "inline", "long","malloc","main","number","new","printf","public","private","protected",
      "return", "short", "signed", "sizeof", "static",
     "struct", "switch", "typedef","this", "union", "unsigned", "void","while"
 };
 //界符运算符表,根据需要可以自行增加
  const char *operatorOrDelimiter[53] = {
     ";", "(", ")", ",", "#",
     "~", "[", "]", "{",
     "}", ".", "?", ":","\'","\"",
	 "+", "++", "+=", "-", "--","-=", "->", "*", "*=",
	 "/", "/=", "%", "%=","<<=", "<<", "<=","<", ">>=", ">>",">=", ">",
	 "&","&&", "&=", "|","||", "|=", "^","^=", "=", "==","!", "!=","\\","\n","\t","\v","\r"
 };
 /********查找保留字*****************/
 int searchReserve(char reserveWord[][20], char s[])
 {
	     for (int i = 0; i < 49; i++)
		     {
		         if (strcmp(reserveWord[i], s) == 0)
			         {//若成功查找，则返回种别码
			             return i + 1;//返回种别码
			         }
		     }
	     return -1;//否则返回-1，代表查找不成功，即为标识符
	 }
 /********查找保留字*****************/

 /*********************判断是否为字母********************/
 bool IsLetter(char letter)
 {//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
	     if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z' || letter == '_')
		     {
		         return true;
		     }
	     else
		     {
		         return false;
		     }
	 }
 /*********************判断是否为字母********************/


 /*****************判断是否为数字************************/
 bool IsDigit(char digit)
 {
	     if (digit >= '0' && digit <= '9')
		     {
		         return true;
		     }
	     else
		     {
		         return false;
		     }
	 }
 /*****************判断是否为数字************************/


 /********************编译预处理，取出无用的字符和注释**********************/
 void filterResource()
 {
	     for (; i <= pProject; )
		     {
			 if (resourceProject[i] == '/' && resourceProject[i + 1] == '/')
			 {//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
				 while (resourceProject[i] != '\n')
				 {   
					
					 i++;//向后扫描
				 } 
				 line_num++;
				 colomn_num = 0;
				 i++;
			 }
			 else if (resourceProject[i] == '/' && resourceProject[i + 1] == '*')
			 {//若为多行注释“/* 。。。*/”则去除该内容
				 i += 2;
				 while (resourceProject[i] != '*' || resourceProject[i + 1] != '/')
				 {
					 i++;//继续扫描
					 if (i >= pProject)
					 {
						 printf("注释出错，没有找到 */，程序结束！！！\n");
						 exit(0);
					 }
				 }
				 i += 2;//跨过“*/”
			 }
			 else if (resourceProject[i] == '\n')
			 {//若出现无用字符，则过滤；否则加载
				 line_num++;
				 colomn_num = 0;
				 i++;
			 }
			 else
				 break;
		     }
	    
	    
	 }
 /********************编译预处理，取出无用的字符和注释**********************/


 /****************************分析子程序，算法核心***********************/
 void Scanner()
 {//根据DFA的状态转换图设计
	     int  count = 0;//count用来做token[]的指示器，收集有用字符
	     char ch;//作为判断使用
		 loop:
		 filterResource();	    
		 for (int j = 0; j < 20; j++)
		     {//每次收集前先清零
		         token[j] = '\0';
		     }
	     ch = resourceProject[i];
	     while (ch == ' ')
		     {//过滤空格，防止程序因识别不了空格而结束
		         i++;
				 goto loop;
		     }
	     if (IsLetter(resourceProject[i]))
		     {//开头为字母
		         token[count++] = resourceProject[i];//收集
		         i++;//下移
				 filterResource();
		         while (IsLetter(resourceProject[i]) || IsDigit(resourceProject[i]))
			         {//后跟字母或数字
			             token[count++] = resourceProject[i];//收集
			             i++;//下移
						 filterResource();
			         }//多读了一个字符既是下次将要开始的指针位置
		         token[count] = '\0';
		         syn = searchReserve(reserveWord, token);//查表找到种别码
		         if (syn == -1)
			         {//若不是保留字则是标识符
			             syn = 121;//标识符种别码
			         }
		         return;
		     }
	     else if (IsDigit(resourceProject[i]))
	     {//首字符为数字
		         while (IsDigit(resourceProject[i])|| resourceProject[i]=='.')
			         {//后跟数字
			             token[count++] = resourceProject[i];//收集
			             i++;
						 filterResource();
			         }//多读了一个字符既是下次将要开始的指针位置
		         token[count] = '\0';
		         syn = 120;//常数种别码
		     }
	     else if (ch == ';' || ch == '(' || ch == ')'
		          || ch == ','|| ch == '#' || ch == '~'|| ch == '['
		          || ch == ']' || ch == '{' || ch == '}'|| ch == '.' || ch == '?' || ch == ':'||ch=='\''||ch=='\"')
		     {//若为运算符或者界符，查表得到结果
		         token[0] = resourceProject[i];
		         token[1] = '\0';//形成单字符串
		         for (int j = 0; j < 15; j++)
			         {//查运算符界符表
			             if (strcmp(token, operatorOrDelimiter[j]) == 0)
				             {
				                 syn = 50 + j;//获得种别码，使用了一点技巧，使之呈线性映射
				                 break;//查到即推出
				             }
			         }
		         i++;//指针下移，为下一扫描做准备
		         return;
		     }
	     else  if (resourceProject[i] == '<')
		     {//<,<=,<<,<<=
		         i++;//后移，超前搜索
				 filterResource();
		         if (resourceProject[i] == '<')
			         {//左移
					     i++;
						 filterResource();
						 if(resourceProject[i] == '=')
						  {
							 syn = 77;
						 }
						 else
						 {
							 i--;
							 syn = 79;
						 }
			           
			         }
				 else if (resourceProject[i] == '=')
			         {
			             syn = 80;
			         }
		         else
			         {
			             i--;
			             syn = 81;
			         }
		         i++;//指针下移
		         return;
		     }
	     else  if (resourceProject[i] == '>')
		     {//>,>=,>>，>>=
		         i++;
				 filterResource();
		         if (resourceProject[i] == '>')
			         {
					     i++;
						 filterResource();
					     if (resourceProject[i] == '=')
					     {
							 syn = 82;
					     }
					     else
					     {
						     i--;
						     syn = 83;
					     }
			         }
				 else if (resourceProject[i] == '=')
				     {
					     syn = 84;
				     }
		         else
			         {
			             i--;
			             syn = 85;
			         }
		         i++;
		         return;
		     }
	     else  if (resourceProject[i] == '=')
		     {//=.==
		         i++;
				 filterResource();
		         if (resourceProject[i] == '=')
			         {
			             syn = 95;
			         }
		         else
			         {
			             i--;
			             syn = 94;
			         }
		         i++;
		         return;
		     }
	     else  if (resourceProject[i] == '!')
		     {//!,!=
		         i++;
				 filterResource();
		         if (resourceProject[i] == '=')
			         {
			             syn = 97;
			         }
		         else
			         {
			             syn = 96;
			             i--;
			         }
		         i++;
		         return;
		     }
	     else  if (resourceProject[i] == '&')
		     {//&,&&,&=
		         i++;
				 filterResource();
		         if (resourceProject[i] == '&')
			         {
			             syn = 87;
			         }
				 else if (resourceProject[i] == '=')
				     {
					     syn = 88;
				     }
		         else
			         {
			             i--;
			             syn = 86;
			         }
		         i++;
		         return;
		     }
	     else  if (resourceProject[i] == '|')
		     {//|,||,|=
		         i++;
				 filterResource();
		         if (resourceProject[i] == '|')
			         {
			             syn = 90;
			         }
				 else if (resourceProject[i] == '=')
				     {
					     syn = 91;
				     }
		         else
			         {
			             i--;
			             syn = 89;
			         }
		         i++;
		         return;
		     }
		 else  if (resourceProject[i] == '+')
				 {//+,++,+=
					 i++;
					 filterResource();
					 if (resourceProject[i] == '+')
					 {
						 syn = 66;
					 }
					 else if (resourceProject[i] == '=')
					 {
						 syn = 67;
					 }
					 else
					 {
						 i--;
						 syn = 65;
					 }
					 i++;
					 return;
				 }
		 else  if (resourceProject[i] == '-')
			 {//-,--,-=,->
			 i++;
			 filterResource();
			 if (resourceProject[i] == '-')
			 {
				 syn = 69;
			 }
			 else if (resourceProject[i] == '=')
			 {
				 syn = 70;
			 }
			 else if (resourceProject[i] == '>')
			 {
				 syn = 71;
			 }
			 else
			 {
				 i--;
				 syn = 68;
			 }
			 i++;
			 return;
			 }
		 else  if (resourceProject[i] == '*')
				 {//*,*=
				 i++;
				 filterResource();
				 if (resourceProject[i] == '=')
				 {
					 syn = 73;
				 }
				 else
				 {
					 i--;
					 syn = 72;
				 }
				 i++;
				 return;
				 }
		 else  if (resourceProject[i] == '/')
				 {///,/=
				 i++;
				 filterResource();
				 if (resourceProject[i] == '=')
				 {
					 syn = 75;
				 }
				 else
				 {
					 i--;
					 syn = 74;
				 }
				 i++;
				 return;
				 }
		 else  if (resourceProject[i] == '%')
				 {//%,%=
				 i++;
				 filterResource();
				 if (resourceProject[i] == '=')
				 {
					 syn = 77;
				 }
				 else
				 {
					 i--;
					 syn = 76;
				 }
				 i++;
				 return;
				 }
		 else  if (resourceProject[i] == '^')
				 {//^,^=
				 i++;
				 filterResource();
				 if (resourceProject[i] == '=')
				 {
					 syn = 93;
				 }
				 else
				 {
					 i--;
					 syn = 92;
				 }
				 i++;
				 return;
				 }
		 else  if (resourceProject[i] == '\\')
				 {//+,++,+=
				 i++;
				 filterResource();
				 if (resourceProject[i] == 'n')
				 {
					 syn = 99;
				 }
				 else
				 {
					 i--;
					 syn = 98;
				 }
				 i++;
				 return;
				 }
	     else  if (resourceProject[i] == EOF)
		     {//结束符
		         syn = 0;//种别码为0
		     }
	     else
		     {//不能被以上词法分析识别，则出错。
		         printf("error：there is no exist %c \n", ch);
		         exit(0);
		     }
	 }


 int main()
 {
	     //打开一个文件，读取其中的源程序
	     FILE * fp, * fp1;
	     if ((fp = fopen("D:\\a.txt", "r")) == NULL)
		     {//打开源程序
		         cout << "can't open this file";
		         exit(0);
		     }
	     resourceProject[pProject] = fgetc(fp);
	     while (resourceProject[pProject] != EOF)
		     {//将源程序读入resourceProject[]数组
		         pProject++;
		         resourceProject[pProject] = fgetc(fp);
		     }
	     resourceProject[++pProject] = '\0';
	     fclose(fp);
		 fp1 = fopen("D:\\b.txt", "w+");
		 cout << setiosflags(ios::right) << setw(12) << "NAME" << setw(10) << "TYPE" << setw(8) << "LINE" << setw(8) << "COLOMN" << endl;
	     while (syn != 0)
		     {
		         //启动扫描
			         Scanner();
					 colomn_num++;
		         if (syn >= 1 && syn <= 74)
			         {//保留字
					     cout << setiosflags(ios::right) << setw(12) << token << setw(10) << syn << setw(8) << line_num << setw(8) << colomn_num << endl;
			             fprintf(fp1, "(%12s%10d%10d%10d)\n", token,syn,line_num, colomn_num);
						 getch();
				 }
		         else if (syn == 120)
			         {//const 常数
					 cout << setiosflags(ios::right) << setw(12) << token << setw(10) << "120" << setw(8) << line_num << setw(8) << colomn_num << endl;
					 fprintf(fp1, "(%12s%10d%10d%10d)\n", token, 120, line_num, colomn_num);
						 getch();
			         }
				 else if (syn == 121)
				 {//标识符
					 cout << setiosflags(ios::right) << setw(12) << token << setw(10) << 121 << setw(8) << line_num << setw(8) << colomn_num << endl;
					 fprintf(fp1, "(%12s%10d%10d%10d)\n", token, 121, line_num, colomn_num);
					 getch();
				 }
				 else if (syn >= 75 && syn <= 97)
				 {
					 cout << setiosflags(ios::right) << setw(12) << operatorOrDelimiter[syn - 50] << setw(10) << syn << setw(8) << line_num << setw(8) << colomn_num << endl;
					 fprintf(fp1, "(%12s%10d%10d%10d)\n", operatorOrDelimiter[syn - 50], syn, line_num, colomn_num);
					 getch();
				 }
				 else if (syn == 98)
				 {
					 cout << setiosflags(ios::right) << setw(12) << setw(12) << operatorOrDelimiter[syn - 50] << setw(10) << "98" << setw(8) << line_num << setw(8) << colomn_num << endl;
					 fprintf(fp1, "(           \s%10d%10d%10d)\n", 98, line_num, colomn_num);
					 getch();
				 }
				 else if (syn == 99)
				 {
					 cout << setiosflags(ios::right) << setw(12) << "\\n" << setw(10) << "99"  << setw(8) << line_num << setw(8) << colomn_num << endl;
					 fprintf(fp1, "(          \\n%10d%10d%10d)\n", 99, line_num, colomn_num);
					 getch();
				 }
		 }
	     fclose(fp1);
	     return 0;
	 }