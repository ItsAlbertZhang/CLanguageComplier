//
//  compiler.h
//  CLC
//
//  Created by Albert Zhang on 2022/1/8.
//

#ifndef compiler_h
#define compiler_h

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define IsTwoCharOperator(a,b) ((a=='='||a=='<'||a=='>'||a=='!')&&b=='='||a=='-'&&b=='>')
#define GetCharType(c) ((c>='0'&&c<='9')*CHAR_NUMBER+(c>='a'&&c<='z'||c>='A'&&c<='Z'||c=='_')*CHAR_LETTER+(c==' ')*CHAR_SPACE+(c=='\t')*CHAR_TABLE+(c=='\n'||c=='\r')*CHAR_RETURN+(c=='\"')*CHAR_DOUBLEQUO+(c=='\'')*CHAR_SINGLEQUO+(c=='('||c==')'||c=='['||c==']'||c=='{'||c=='}'||c==';'||c==','||c=='/')*CHAR_SEPARATOR)
//获取c对应的enum值
/*
 对于任意字符串,对其进行检查:
 如果第一个字符为运算符,则其为运算符,长度可能为1或2
 如果第一个字符为分隔符,则其为分隔符,长度可能为1或2
 如果第一个字符为数字,则其为整型常量或浮点型常量,长度为直至下一个字符不是数字且不是小数点
 如果第一个字符为"双引号,则其为字符串常量,长度为直至下一个字符是双引号
 如果第一个字符为'单引号,则其为字符常量,长度为3
 如果第一个字符为字母或下划线,则其为变量名,长度为直至下一个字符不是字母、不是数字、也不是下划线
 除此之外,还应考虑空格,\t,\n.
 */
enum CharType {
    CHAR_OPERATOR,      //运算符
    CHAR_NUMBER,        //数字
    CHAR_LETTER,        //字母和下划线
    CHAR_SPACE,         //空格
    CHAR_TABLE,         //制表符
    CHAR_RETURN,        //回车
    CHAR_DOUBLEQUO,     //双引号
    CHAR_SINGLEQUO,     //单引号
    CHAR_SEPARATOR,     //分隔符(括号,分号,逗号,注释)
};



typedef struct TK {
    //存储一个单词
    int token;  //单词对应的token编码
    int row;    //单词所在行
    char *s;    //单词内容
} TKword;

typedef struct TKLinkedList {
    //存储单词链表
    TKword word;
    struct TKLinkedList *next;
} node;

enum TokenCode {
    //运算符
    TK_PLUS,         // + 加号
    //注意, 从1开始编号
    TK_MINUS,       // - 减号
    TK_MUL,         // * 乘号
    TK_DIVIDE,      // / 除号
    TK_MOD,         // % 求余运算符
    TK_ASSIGN,      // = 赋值运算符
    TK_EQ,          // == 等于号
    TK_NEQ,         // != 不等于号
    TK_LT,          // < 小于号
    TK_LEQ,         // <= 小于等于号
    TK_GT,          // > 大于号
    TK_GEQ,         // >= 大于等于号
    TK_POINTSTO,    // -> 指向结构体成员运算符
    TK_DOT,         // . 结构体成员运算符
    TK_AND,         // & 地址与运算符

    //分隔符
    TK_OPENPA,      // ( 左圆括号
    TK_CLOSEPA,     // ) 右圆括号
    TK_OPENBR,      // [ 左中括号
    TK_CLOSEBR,     // ] 右中括号
    TK_BEGIN,       // { 左大括号
    TK_END,         // } 右大括号
    TK_SEMICOLON,   // ; 分号
    TK_COMMA,       // , 逗号
    TK_REF,         // //注释

    //常量
    TK_CINT,        // 整型常量
    TK_CFLOAT,      // 浮点型常量
    TK_CCHAR,       // 字符常量
    TK_CSTR,        // 字符串常量

    //变量类型
    KW_CHAR,        // char关键字
    KW_SHORT,       // short关键字
    KW_INT,         // int关键字
    KW_VOID,        // void关键字
    KW_STRUCT,      // struct关键字

    //关键字
    KW_IF,          // if关键字
    KW_ELSE,        // else关键字
    KW_FOR,         // for关键字
    KW_WHILE,       // while关键字
    KW_CONTINUE,    // continue关键字
    KW_BREAK,       // break关键字
    KW_RETURN,      // return关键字
    KW_SIZEOF,      // sizeof关键字

    //变量与函数名
    TK_IDENTV,      //变量名
    TK_IDENTF,      //函数名
};

enum Color {
    COLOR_OPERATOR,     //运算符和分号逗号,用rgbi
    COLOR_SEPARATOR,    //括号,用rbi
    COLOR_REF,          //注释,用i
    COLOR_NUM,          //数字常量,用bg
    COLOR_CHAR,         //字符和字符串常量,用gr
    COLOR_KEYWORD,      //关键字,用ri
    COLOR_IDENTV,       //变量名,用bgi
    COLOR_IDENTF,       //函数名,用gri
};

//拉链法构造哈希表, 用于快速查询是否已有该单词, 对于已有的单词快速查询其token编码
typedef struct HashNode {
    //哈希表表体
    int token;              //单词对应的token编码
    char word[100];         //单词前100个字符
    struct HashNode *next;  //拉链上的下一个单词
} hashnode;
#define HASH_MAX 1000
/*
 这里使用了带头结点的链表.原因在于不带头结点的链表在查询等操作时,需要考虑查询操作会比偏移操作多一次的问题.
 (指针从第1个结点移动到第n个结点,移动次数共n-1,而不带头结点的情况下实际信息数为n.)
 */

//library.c
//单词链表的头节点初始化
int NodeInit(node * head);
//单词链表的结点新增
node* NodeAdd(node * anynode, int token, int row, char *s, int len);
//单词链表的结点值查找(char* node->word->s)
char* NodeGetWord(node * thisnode);
//哈希处理
int elf_hash(char* s, int strlen);
//已知词哈希表的初始化
int HashInit(hashnode *arr, int len);
//已知词哈希表的查询(S,search)和添加(A,add)
int HashSA(hashnode *arr, int hashtablelen, char *s, int strlen);
//已知词哈希表初始化插入
int HashInsert(hashnode *arr, int hashtablelen, char *s, int strlen, int token);
//两个已知长度但无结束符的字符串比较
int mystrcmp(const char *str1, const char *str2, int len);

//lexical_analysis.c
//将字符串分割为单词存储, 并将其打印输出
node* split(char *s, node *anynode, hashnode *hashtab, int hashtablen, int row);
//查询某单词的Token编码
int GetToken(char *s, int strlen);
//中间调用打印函数,Xcode环境下需要注释其中部分代码
void PrintWithColor(char *s, int len, int color);
//初始化已知词哈希表为CLC环境(将C语言关键词放入已知词哈希表)
int HashInitCLCkey(hashnode *arr, int len);

//print.c
//Windows下的打印函数
void PrintAtWindows(char *s, int len, int color);


#endif /* compiler_h */
