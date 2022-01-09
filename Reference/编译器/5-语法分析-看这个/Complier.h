/*
* 虚假编译器 VC 6.0 (Virtual Compiler 6.0)
*
* @author Underwater
* @1/12/2020
*
* ——————声明———————
* 程序基于C++语言
* 参考了样例架构和枚举类型，参考了课件的表达式递归分析函数
* 剩余功能自行实现
* 运行环境为Microsoft Visual Studio Community 2019 版本 16.4.2
*
* ——————更新———————
* 新增while语句体判断
* 新增注释功能
* 新增函数返回类型判断
* 新增if-else逻辑判断和括号匹配
* etc.
*
* ——————缺陷———————
* 不支持struct/数组/指针及指针相关操作
* 不支持浮点型和字符串类型
* 不支持++/--/！/||/&&
* 不支持break和continue在语句体内部的逻辑判断
* 不支持检测未定义的变量与函数名
* 存在众多尚未检测出的bug
* 程序无注释和操作手册
*/

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<csetjmp>
#include<string>
#include <functional>
#include<vector>
#include<iostream>
#include<Windows.h>
#include<stack>
using namespace std;

#define IS_LETTER_OR_NUM(c) ('a'<=c&&c<='z'||'A'<=c&&c<='Z'||'0'<=c&&c<='9')
#define IS_LETTER(c) ('a'<=c&&c<='z'||'A'<=c&&c<='Z')
#define IS_NUM(c) ('0'<=c&&c<='9')
#define IS_TYPE(sym) (KW_CHAR<=sym&&sym<=KW_VOID)
#define MAX 1000


typedef struct TK {
	int token;
	string s;		
	int row;
}TkWord, * pTkWord;

enum TokenCode
{
	/* 运算符及分隔符 */
	TK_PLUS,		// + 加号
	TK_MINUS,		// - 减号
	TK_MUL,			// * 乘号
	TK_DIVIDE,		// / 除号
	TK_MOD,			// % 求余运算符
	TK_ASSIGN,		// = 赋值运算符 
	TK_EQ,			// == 等于号
	TK_NEQ,			// != 不等于号
	TK_LT,			// < 小于号
	TK_LEQ,			// <= 小于等于号
	TK_GT,			// > 大于号
	TK_GEQ,			// >= 大于等于号
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
	TK_REF,			// //注释
	TK_ELLIPSIS,	// ... 省略号
	TK_EOF,			// 文件结束符

	/* 常量 */
	TK_CINT,		// 整型常量
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
	KW_WHILE,		// while关键字
	KW_CONTINUE,	// continue关键字
	KW_BREAK,		// break关键字   
	KW_RETURN,		// return关键字
	KW_SIZEOF,		// sizeof关键字

	KW_ALIGN,		// __align关键字	
	KW_CDECL,		// __cdecl关键字 standard c call
	KW_STDCALL,     // __stdcall关键字 pascal c call

	/* 标识符 */
	TK_IDENTV,//变量名
	TK_IDENTF//函数名
};

enum Error_type {
	PROGRAM_BEGIN,		//执行语法分析
	SUCC,				//成功
	SOMETHINGWRONG,		//未知错误
	LACK_SEMICOLON,		//缺少分号
	RETURN_ERROR,		//return类型错误
	BREAK_LACK_SEMICOLON,		//break缺少分号
	CONTINUE_LACK_SEMICOLON,	//continue缺少分号
	EXTER_DEC,			//外部声明处出现错误
	LACK_TYPE,			//缺少类型
	VRA_WRONG,			//变量错误
	FUNC_WRONG,			//函数错误
	LACK_IDENT,			//缺少标识符
	LACK_OPENPA,		//缺少左圆括号
	LACK_CLOSEPA,		//缺少右圆括号
	LACK_OPENBR,		//缺少左中括号
	LACK_CLOSEBR,		//缺少右中括号
	LACK_BEGIN,			//缺少左花括号
	LACK_END,			//缺少右花括号
	NESTED_FUNC_DECLEAR,//函数嵌套定义
	IF_LACK_OPENPA,		//if缺少左圆括号
	IF_LACK_CLOSEPA,	//if缺少右圆括号
	FOR_LACK_OPENPA,	//for缺少左圆括号
	FOR_LACK_CLOSEPA,	//for缺少右圆括号
	LACK_PRIMARY,		//初等表达式错误
	FUNCVAR_WRONG,		//函数调用变量错误
	LACK_ST,			//缺少语句
	CONTINUE_ERROR,		//continue使用错误
	ELSE_ERROR,			//else使用错误
	FOR_ERROR,			//for使用错误
	WHILE_LACK_OPENPA,	//while缺少左圆括号
	WHILE_LACK_CLOSEPA,	//while缺少右圆括号
	WHILE_ERROR,		//while使用错误
};

/*---------------------------------词法分析函数----------------------------------------*/
void Print(string ch, int token);
void PrintWhite();
void tmpClera(pTkWord tmp);

/*---------------------------------语法分析函数----------------------------------------*/
void Compile();
void NextNode();
void External_dec();
void type_Statement();
void Variate(int i);
void Func();
void Func_Virate();
void Func_Body();
void Func_char();
void Func_int();
void Func_func();
void VariInFR();
void For_Process();
void If_Process();
void Else_Process();
void While_Process();
void Return_Process();
void PrintBegin();
void Primary_Assign();
void Next();
void E();
void C();
void T();
void F();