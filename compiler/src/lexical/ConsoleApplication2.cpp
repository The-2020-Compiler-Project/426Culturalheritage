 #include "stdio.h"
 #include "stdlib.h"
 #include "string.h"
 #include "iostream"
 using namespace std;
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
 void filterResource(char r[], int pProject)
 {
	     char tempString[10000];
	     int count = 0;
	     for (int i = 0; i <= pProject; i++)
		     {
		         if (r[i] == '/' && r[i + 1] == '/')
			         {//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
			             while (r[i] != '\n')
				             {
				                 i++;//向后扫描
				             }
			         }
		         if (r[i] == '/' && r[i + 1] == '*')
			         {//若为多行注释“/* 。。。*/”则去除该内容
			             i += 2;
			             while (r[i] != '*' || r[i + 1] != '/')
				             {
				                 i++;//继续扫描
				                 if (r[i] == '$')
					                 {
					                     printf("注释出错，没有找到 */，程序结束！！！\n");
					                     exit(0);
					                 }
				             }
			             i += 2;//跨过“*/”
			         }
		         if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
			         {//若出现无用字符，则过滤；否则加载
			             tempString[count++] = r[i];
			         }
		     }
	     tempString[count] = '\0';
	     strcpy(r, tempString);//产生净化之后的源程序
	 }
 /********************编译预处理，取出无用的字符和注释**********************/


 /****************************分析子程序，算法核心***********************/
 void Scanner(int& syn, char resourceProject[], char token[], int& pProject)
 {//根据DFA的状态转换图设计
	     int i, count = 0;//count用来做token[]的指示器，收集有用字符
	     char ch;//作为判断使用
	     ch = resourceProject[pProject];
	     while (ch == ' ')
		     {//过滤空格，防止程序因识别不了空格而结束
		         pProject++;
		         ch = resourceProject[pProject];
		     }
	     for (i = 0; i < 20; i++)
		     {//每次收集前先清零
		         token[i] = '\0';
		     }
	     if (IsLetter(resourceProject[pProject]))
		     {//开头为字母
		         token[count++] = resourceProject[pProject];//收集
		         pProject++;//下移
		         while (IsLetter(resourceProject[pProject]) || IsDigit(resourceProject[pProject]))
			         {//后跟字母或数字
			             token[count++] = resourceProject[pProject];//收集
			             pProject++;//下移
			         }//多读了一个字符既是下次将要开始的指针位置
		         token[count] = '\0';
		         syn = searchReserve(reserveWord, token);//查表找到种别码
		         if (syn == -1)
			         {//若不是保留字则是标识符
			             syn = 121;//标识符种别码
			         }
		         return;
		     }
	     else if (IsDigit(resourceProject[pProject]))
	     {//首字符为数字
		         while (IsDigit(resourceProject[pProject])|| resourceProject[pProject]=='.')
			         {//后跟数字
			             token[count++] = resourceProject[pProject];//收集
			             pProject++;
			         }//多读了一个字符既是下次将要开始的指针位置
		         token[count] = '\0';
		         syn = 120;//常数种别码
		     }
	     else if (ch == ';' || ch == '(' || ch == ')'
		          || ch == ','|| ch == '#' || ch == '~'|| ch == '['
		          || ch == ']' || ch == '{' || ch == '}'|| ch == '.' || ch == '?' || ch == ':'||ch=='\''||ch=='\"')
		     {//若为运算符或者界符，查表得到结果
		         token[0] = resourceProject[pProject];
		         token[1] = '\0';//形成单字符串
		         for (i = 0; i < 15; i++)
			         {//查运算符界符表
			             if (strcmp(token, operatorOrDelimiter[i]) == 0)
				             {
				                 syn = 50 + i;//获得种别码，使用了一点技巧，使之呈线性映射
				                 break;//查到即推出
				             }
			         }
		         pProject++;//指针下移，为下一扫描做准备
		         return;
		     }
	     else  if (resourceProject[pProject] == '<')
		     {//<,<=,<<,<<=
		         pProject++;//后移，超前搜索
		         
		         if (resourceProject[pProject] == '<')
			         {//左移
					     pProject++;
						 if(resourceProject[pProject] == '=')
						  {
							 syn = 77;
						 }
						 else
						 {
							 pProject--;
							 syn = 79;
						 }
			           
			         }
				 else if (resourceProject[pProject] == '=')
			         {
			             syn = 80;
			         }
		         else
			         {
			             pProject--;
			             syn = 81;
			         }
		         pProject++;//指针下移
		         return;
		     }
	     else  if (resourceProject[pProject] == '>')
		     {//>,>=,>>，>>=
		         pProject++;
		        
		         if (resourceProject[pProject] == '>')
			         {
					     pProject++;
					     if (resourceProject[pProject] == '=')
					     {
							 syn = 82;
					     }
					     else
					     {
						     pProject--;
						     syn = 83;
					     }
			         }
				 else if (resourceProject[pProject] == '=')
				     {
					     syn = 84;
				     }
		         else
			         {
			             pProject--;
			             syn = 85;
			         }
		         pProject++;
		         return;
		     }
	     else  if (resourceProject[pProject] == '=')
		     {//=.==
		         pProject++;
		         if (resourceProject[pProject] == '=')
			         {
			             syn = 95;
			         }
		         else
			         {
			             pProject--;
			             syn = 94;
			         }
		         pProject++;
		         return;
		     }
	     else  if (resourceProject[pProject] == '!')
		     {//!,!=
		         pProject++;
		         if (resourceProject[pProject] == '=')
			         {
			             syn = 97;
			         }
		         else
			         {
			             syn = 96;
			             pProject--;
			         }
		         pProject++;
		         return;
		     }
	     else  if (resourceProject[pProject] == '&')
		     {//&,&&,&=
		         pProject++;
		         if (resourceProject[pProject] == '&')
			         {
			             syn = 87;
			         }
				 else if (resourceProject[pProject] == '=')
				     {
					     syn = 88;
				     }
		         else
			         {
			             pProject--;
			             syn = 86;
			         }
		         pProject++;
		         return;
		     }
	     else  if (resourceProject[pProject] == '|')
		     {//|,||,|=
		         pProject++;
		         if (resourceProject[pProject] == '|')
			         {
			             syn = 90;
			         }
				 else if (resourceProject[pProject] == '=')
				     {
					     syn = 91;
				     }
		         else
			         {
			             pProject--;
			             syn = 89;
			         }
		         pProject++;
		         return;
		     }
		 else  if (resourceProject[pProject] == '+')
				 {//+,++,+=
					 pProject++;
					 if (resourceProject[pProject] == '+')
					 {
						 syn = 66;
					 }
					 else if (resourceProject[pProject] == '=')
					 {
						 syn = 67;
					 }
					 else
					 {
						 pProject--;
						 syn = 65;
					 }
					 pProject++;
					 return;
				 }
		 else  if (resourceProject[pProject] == '-')
			 {//-,--,-=,->
			 pProject++;
			 if (resourceProject[pProject] == '-')
			 {
				 syn = 69;
			 }
			 else if (resourceProject[pProject] == '=')
			 {
				 syn = 70;
			 }
			 else if (resourceProject[pProject] == '>')
			 {
				 syn = 71;
			 }
			 else
			 {
				 pProject--;
				 syn = 68;
			 }
			 pProject++;
			 return;
			 }
		 else  if (resourceProject[pProject] == '*')
				 {//*,*=
				 pProject++;
				 if (resourceProject[pProject] == '=')
				 {
					 syn = 73;
				 }
				 else
				 {
					 pProject--;
					 syn = 72;
				 }
				 pProject++;
				 return;
				 }
		 else  if (resourceProject[pProject] == '/')
				 {///,/=
				 pProject++;
				 if (resourceProject[pProject] == '=')
				 {
					 syn = 75;
				 }
				 else
				 {
					 pProject--;
					 syn = 74;
				 }
				 pProject++;
				 return;
				 }
		 else  if (resourceProject[pProject] == '%')
				 {//%,%=
				 pProject++;
				 if (resourceProject[pProject] == '=')
				 {
					 syn = 77;
				 }
				 else
				 {
					 pProject--;
					 syn = 76;
				 }
				 pProject++;
				 return;
				 }
		 else  if (resourceProject[pProject] == '^')
				 {//^,^=
				 pProject++;
				 if (resourceProject[pProject] == '=')
				 {
					 syn = 93;
				 }
				 else
				 {
					 pProject--;
					 syn = 92;
				 }
				 pProject++;
				 return;
				 }
		 else  if (resourceProject[pProject] == '\\')
				 {//+,++,+=
				 pProject++;
				 if (resourceProject[pProject] == 'n')
				 {
					 syn = 99;
				 }
				 else if (resourceProject[pProject] == 't')
				 {
					 syn = 100;
				 }
				 else if (resourceProject[pProject] == 'v')
				 {
					 syn = 101;
				 }
				 else if (resourceProject[pProject] == 'r')
				 {
					 syn = 102;
				 }
				 else
				 {
					 pProject--;
					 syn = 98;
				 }
				 pProject++;
				 return;
				 }
	     else  if (resourceProject[pProject] == EOF)
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
		     char resourceProject[10000];
	     char token[20] = { 0 };
	     int syn = -1, i;//初始化
	     int pProject = 0;//源程序指针
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
	     cout << endl << "源程序为:" << endl;
	     cout << resourceProject << endl;
	     //对源程序进行过滤
		     filterResource(resourceProject, pProject);
	     cout << endl << "过滤之后的程序:" << endl;
	     cout << resourceProject << endl;
	     pProject = 0;//从头开始读
	
		 fp1 = fopen("D:\\b.txt", "w+");
		 cout << endl << endl;
		     
	     while (syn != 0)
		     {
		         //启动扫描
			         Scanner(syn, resourceProject, token, pProject);
		        
		         if (syn >= 1 && syn <= 74)
			         {//保留字
			             printf("(%s   ,  %d)\n", token,syn);
			             fprintf(fp1, "(%s   ,  %d)\n", token,syn);
			         }
		         else if (syn == 120)
			         {//const 常数
			             printf("(%s   ,   120)\n", token);
			             fprintf(fp1, "(%s   ,   120)\n", token);
			         }
				 else if (syn == 121)
				 {//标识符
					 printf("(%s   ,   121)\n", token);
					 fprintf(fp1, "(%s   ,   121)\n", token);
				 }
				 else if (syn >= 75 && syn <= 97)
				 {
					 printf("(%s   ,   %d)\n", operatorOrDelimiter[syn - 50],syn);
					 fprintf(fp1, "(%s   ,   %d)\n", operatorOrDelimiter[syn - 50],syn);
				 }
				 else if (syn == 98)
				 {
					 printf("(\   ,   98)\n", operatorOrDelimiter[syn - 50]);
					 fprintf(fp1, "(\   ,   98)\n");
				 }
				 else if (syn == 99)
				 {
					 printf("(\\n   ,   99)\n");
					 fprintf(fp1, "(\\n   ,   99)\n");
				 }
				 else if (syn == 100)
				 {
					 printf("(\\t   ,  100)\n");
					 fprintf(fp1, "(\\t   ,   100)\n");
				 }
				 else if (syn == 101)
				 {
					 printf("(\\v   ,   101)\n");
					 fprintf(fp1, "(\\v   ,   101)\n");
				 }
				 else if (syn == 102)
				 {
					 printf("(\\r   ,   102)\n");
					 fprintf(fp1, "(\\r   ,   102)\n");
				 }
		 }
	     fclose(fp1);
	     return 0;
	 }