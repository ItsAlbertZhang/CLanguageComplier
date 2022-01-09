#include <Windows.h>
#include "complier.h"

TkWord *tk_HashTable[MAXKEY] = { 0 };//单词的哈希表
//存放字符串的动态数组
vector<string> word;
//这个handle是为了颜色打印
HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
//把每一中符号进行token编码，都有对应的数字值
static TkWord keywords[]={
	{TK_PLUS,"+",NULL},
	{TK_MINUS,"-",NULL},
	{TK_STAR,"*",NULL},
	{TK_DIVIDE,"/",NULL},
	{TK_MOD,"%",NULL},
	{TK_EQ,"==",NULL},
	{TK_NEQ,"!=",NULL},
	{TK_LT,"<",NULL},
	{TK_LEQ,"<=",NULL},
	{TK_GT,">",NULL},
	{TK_GEQ,">=",NULL},
	{TK_ASSIGN,"=",NULL},
	{TK_POINTSTO,"->",NULL},
	{TK_DOT,".",NULL},
	{TK_AND,"&",NULL},
	{TK_OPENPA,"(",NULL},
	{TK_CLOSEPA,")",NULL},
	{TK_OPENBR,"[",NULL},
	{TK_CLOSEBR,"]",NULL},
	{TK_BEGIN,"{",NULL},
	{TK_END,"}",NULL},
	{TK_SEMICOLON,";",NULL},
	{TK_COMMA,",",NULL},
	{TK_ELLIPSIS,"...",NULL},
	{TK_EOF,"End Of File",NULL},

	{TK_CINT,"整形常量",NULL},
	{TK_CFLOAT,"浮点型常量",NULL},
	{TK_CCHAR,"字符常量",NULL},
	{TK_CSTR,"字符串常量",NULL},

	{KW_CHAR,"char",NULL},
	{KW_SHORT,"short",NULL},
	{KW_INT,"int",NULL},
	{KW_VOID,"void",NULL},
	{KW_STRUCT,"struct",NULL},

	{KW_IF,"if",NULL},
	{KW_ELSE,"else",NULL},
	{KW_FOR,"for",NULL},
	{KW_CONTINUE,"continue",NULL},
	{KW_BREAK,"break",NULL},
	{KW_RETURN,"return",NULL},
	{40}//TK_IDENT
};

//优势，代码很短，清晰
//劣势，其实他基础不错，还可以写语法分析的
int main(){
	int keywordsLen = sizeof(keywords) / sizeof(TkWord);
	InitKeywords(keywords, keywordsLen, tk_HashTable);//初始化哈希表
	ifstream file;
	file.open("test.txt",ios::in);//FILE *fp=fopen("test.txt","r+");
	if(!file.is_open()){//文件打开失败，返回
		return 0;
	}
	string str;//char str[1000];//存一行字符串
	
	while(getline(file,str)){//一次读取一行，fgets(str,sizeof(str),fp)
		if(str.empty()){
			continue;
		}
		word.clear();
		//split是把每一行剥离为一个一个的单词
		int err_num=split(word,str);
		for(int i=0;i<word.size();i++){
			string temp=word.at(i);
			//cout<<temp;
			pTkWord cur=tk_HashTable[elf_hash(temp)];
			if(NULL!=cur&&cur->spelling==temp){//是在hash table中的内容
				tokencode token=(tokencode)cur->tkcode;
				printColor(temp,token);
			}else if(temp[0]==34||isdigit(temp[0])||temp[0]==39){//整型常量、浮点型常量、字符串常量、字符常量
				SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN);
				cout<<temp;
			}else{//空格 变量名 或者函数名
				SetConsoleTextAttribute(h,FOREGROUND_INTENSITY);
				cout<<temp;
			}
		}
		cout<<endl;
	}
	system("pause");
	return 0;
}