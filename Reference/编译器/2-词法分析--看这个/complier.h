#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<string.h>
#include<ctype.h>
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;
#define MAXKEY 1000

enum e_TokenCode
{
	/* 运算符及分隔符 */
	TK_PLUS,		// + 加号
	TK_MINUS,		// - 减号
	TK_STAR,		// * 星号
	TK_DIVIDE,		// / 除号
	TK_MOD,			// % 求余运算符
	TK_EQ,			// == 等于号
	TK_NEQ,			// != 不等于号
	TK_LT,			// < 小于号
	TK_LEQ,			// <= 小于等于号
	TK_GT,			// > 大于号
	TK_GEQ,			// >= 大于等于号
	TK_ASSIGN,		// = 赋值运算符 
	TK_POINTSTO,	// -> 指向结构体成员运算符
	TK_DOT,			// . 结构体成员运算符
	TK_AND,         // & 地址与运算符
	TK_OPENPA,		// ( 左圆括号
	TK_CLOSEPA,		// ) 右圆括号
	TK_OPENBR,		// [ 左中括号
	TK_CLOSEBR,		// ] 右中括号
	TK_BEGIN,		// { 左大括号
	TK_END,			// } 右大括号
	TK_SEMICOLON,	// ; 分号    
	TK_COMMA,		// , 逗号
	TK_ELLIPSIS,	// ... 省略号
	TK_EOF,			// 文件结束符

    /* 常量 */
    TK_CINT,		// 整型常量
	TK_CFLOAT,		// 浮点型常量
    TK_CCHAR,		// 字符常量
    TK_CSTR,		// 字符串常量

	/* 关键字 */
	KW_CHAR,		// char关键字
	KW_SHORT,		// short关键字
	KW_INT,			// int关键字
	KW_VOID,		// void关键字  
	KW_STRUCT,		// struct关键字   
	KW_IF,			// if关键字
	KW_ELSE,		// else关键字
	KW_FOR,			// for关键字
	KW_CONTINUE,	// continue关键字
	KW_BREAK,		// break关键字   
	KW_RETURN,		// return关键字

	/* 标识符 */
	TK_IDENT
};

typedef enum e_TokenCode tokencode;

typedef struct TkWord{
	int tkcode;	//单词编码
	string spelling;	//单词字符串,如果用char spell[200];搞大点
	struct TkWord *next;	//指向哈希冲突的其他单词
}TkWord,*pTkWord;

int elf_hash(string key);
void printColor(string str,tokencode token);
void InitKeywords(pTkWord keywords,int keywordsLen,pTkWord *tk_Hashtable);
int split(vector<string> &word,const string str);//分割一行