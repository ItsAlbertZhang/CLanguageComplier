//
//  compiler.h
//  CLC
//
//  Created by Albert Zhang on 2022/1/8.
//

#ifndef compiler_h
#define compiler_h

//#define _CRT_SECURE_NO_WARNINGS
//#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct TK {
    //存储一个单词
    int token;  //单词对应的token编码
    int row;    //单词所在行
    char *s;    //单词内容
    int len;    //单词长度
} TKword;

typedef struct TKLinkedList {
    //存储单词链表
    TKword word;
    struct TKLinkedList *next;
} node;

enum TokenCode {
    //运算符
    TK_PLUS = 1,    // + 加号
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

//拉链法构造哈希表, 用于快速查询是否已有该单词, 对于已有的单词快速查询其token编码
typedef struct HashNode {
    //哈希表表体
    int token;              //单词对应的token编码
    char word[10];          //单词前10个字符
    struct HashNode *next;  //拉链上的下一个单词
} hashnode;

//library.c
//单词链表的头节点初始化
int NodeInit(node * head);
//单词链表的结点新增
node* NodeAdd(node * anynode, int token, int row, char *s, int len);
//单词链表的结点值查找(char* node->word->s)
char* NodeGetWord(node * thisnode);
//已知词哈希表的初始化
int HashInit(hashnode **arr, int len);

//lexical_analysis.c
//将字符串分割为单词存储, 并将其打印输出
node* split(char *str, node *anynode);


#endif /* compiler_h */
