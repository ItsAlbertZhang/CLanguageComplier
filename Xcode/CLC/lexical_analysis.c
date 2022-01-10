//
//  lexical_analysis.c
//  CLC
//  词法分析源文件
//  Created by Albert Zhang on 2022/1/8.
//

#include "compiler.h"

//将字符串分割为单词存储, 并将其打印输出
node* split(char *s, node *anynode, hashnode *hashtable, int hashtablelen, int row) {
    node *lastnode = anynode;
    while(lastnode->next) {
        lastnode = lastnode->next;
    }   //定义一个链表结点指针并使其指向链表末尾结点
    char *fast = s, *slow = s;  //定义一个慢指针与一个快指针
    int slowtype = 0, token = 0;
    int temp = 0;
    while(*slow) {  //当慢指针未指向字符串末尾时
        slowtype = GetCharType(*slow);  //获取慢指针指向的字符的类型
        switch (slowtype) {
            case CHAR_OPERATOR: //情况0:慢指针指向一个运算符
                fast++;
                if(IsTwoCharOperator(*slow, *fast)) {   //慢指针和它后面的字符是>=,<=,==,!=或->中的一种,此时两个运算符为一体
                    token = HashSA(hashtable, hashtablelen, slow, 2);   //哈希查询其token编码,且必定命中
                    lastnode = NodeAdd(lastnode, token, row, slow, 2);
                    fast++;
                } else {    //慢指针单字符构成运算符
                    token = HashSA(hashtable, hashtablelen, slow, 1);   //哈希查询其token编码,且必定命中
                    lastnode = NodeAdd(lastnode, token, row, slow, 1);
                }
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_NUMBER:   //情况1:慢指针指向一个数字
                temp = 1;   //临时变量用于存储小数点的出现次数
                while(GetCharType(*fast) == CHAR_NUMBER || temp * (*fast == '.')) { //当快指针指向数字或是小数点时(仅限一次)
                    if(*fast == '.') {
                        temp = 0;   //指向小数点则将临时变量标记为小数点已出现
                    }
                    fast++; //快指针向后移动
                }   //结束循环时,快指针的前一位为数字的最后一位
                if(temp) {
                    token = TK_CINT;
                } else {
                    token = TK_CFLOAT;
                }   //根据小数点是否出现过判断该数字是整型还是浮点型,对应不同的token编码
                lastnode = NodeAdd(lastnode, token, row, slow, (int)(fast - slow));
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_LETTER:   //情况2:慢指针指向一个字母或下划线
                //这种情况较为复杂,原因在于单词的哈希查询可能未命中,此时需要判断其为变量名还是函数名
                while(GetCharType(*fast) == CHAR_LETTER || GetCharType(*fast) == CHAR_NUMBER) {
                    fast++;
                }   //结束循环时,快指针的前一位为单词的最后一位
                token = HashSA(hashtable, hashtablelen, slow, (int)(fast - slow));  //哈希查询其token编码,此时可能未命中,需要在HashSA函数依赖的GetToken函数中额外考虑
                lastnode = NodeAdd(lastnode, token, row, slow, (int)(fast - slow));
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_SPACE:    //情况3:慢指针指向空格
                //空格不储存
                fast++;
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_TABLE:    //情况4:慢指针指向制表符
                //制表符不储存
                fast++;
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_RETURN:    //情况5:慢指针指向回车
                //回车不储存
                fast++;
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_DOUBLEQUO:    //情况6:慢指针指向双引号
                fast++;
                while(*fast++ != '\"'); //循环结束时,快指针的前一位为后双引号
                token = TK_CSTR;
                lastnode = NodeAdd(lastnode, token, row, slow, (int)(fast - slow));
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_SINGLEQUO:    //情况7:慢指针指向单引号
                fast = slow + 3;
                token = TK_CCHAR;
                lastnode = NodeAdd(lastnode, token, row, slow, (int)(fast - slow));
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
            case CHAR_SEPARATOR:    //情况8:慢指针指向运算符
                fast++;
                if(*slow == '/' && *fast == '/') {    //之后字符串的全部为注释
                    token = TK_REF;
                    while(*++fast);
                } else {
                    token = HashSA(hashtable, hashtablelen, slow, 1);   //哈希查询其token编码,且必定命中
                    lastnode = NodeAdd(lastnode, token, row, slow, 1);
                }
                PrintWithColor(slow, (int)(fast - slow), token);
                slow = fast;
                break;
        }
    }
    return lastnode;
}

//哈希查询未命中的情况下,判断某单词的Token编码
int GetToken(char *slow, int strlen) {
    int ret = 0;
    char *fast = slow + strlen; //end指向该单词之后的第一个字母(此时s和end所指向的字符串是main函数中的s[1000])
    int slowtype = GetCharType(*slow);
    switch (slowtype) {
        case CHAR_LETTER:   //对应spilt中的情况3
            while(*fast && *fast == ' ') {
                fast++;
            }   //fast指向第一个不是空格的字符
            if(*fast == '(') {
                ret = TK_IDENTF;    //如果fast指向左括号,说明该单词为函数名
            } else {
                ret = TK_IDENTV;    //否则说明该单词为变量名
            }
            break;
    }
    return ret;
}

//初始化已知词哈希表为CLC环境(将C语言关键词放入已知词哈希表)
int HashInitCLCkey(hashnode *arr, int len) {
    HashInsert(arr, len, "+", 1, TK_PLUS);
    HashInsert(arr, len, "-", 1, TK_MINUS);
    HashInsert(arr, len, "*", 1, TK_MUL);
    HashInsert(arr, len, "/", 1, TK_DIVIDE);
    HashInsert(arr, len, "%", 1, TK_MOD);
    HashInsert(arr, len, "=", 1, TK_ASSIGN);
    HashInsert(arr, len, "==", 2, TK_EQ);
    HashInsert(arr, len, "!=", 2, TK_NEQ);
    HashInsert(arr, len, "<", 1, TK_LT);
    HashInsert(arr, len, "<=", 2, TK_LEQ);
    HashInsert(arr, len, ">", 1, TK_GT);
    HashInsert(arr, len, ">=", 2, TK_GEQ);
    HashInsert(arr, len, "->", 2, TK_POINTSTO);
    HashInsert(arr, len, ".", 1, TK_DOT);
    HashInsert(arr, len, "&", 1, TK_AND);
    HashInsert(arr, len, "(", 1, TK_OPENPA);
    HashInsert(arr, len, ")", 1, TK_CLOSEPA);
    HashInsert(arr, len, "[", 1, TK_OPENBR);
    HashInsert(arr, len, "]", 1, TK_CLOSEBR);
    HashInsert(arr, len, "{", 1, TK_BEGIN);
    HashInsert(arr, len, "}", 1, TK_END);
    HashInsert(arr, len, ";", 1, TK_SEMICOLON);
    HashInsert(arr, len, ",", 1, TK_COMMA);
    HashInsert(arr, len, "//", 2, TK_REF);
    HashInsert(arr, len, "char", 4, KW_CHAR);
    HashInsert(arr, len, "short", 5, KW_SHORT);
    HashInsert(arr, len, "int", 3, KW_INT);
    HashInsert(arr, len, "void", 4, KW_VOID);
    HashInsert(arr, len, "struct", 6, KW_STRUCT);
    HashInsert(arr, len, "if", 2, KW_IF);
    HashInsert(arr, len, "else", 4, KW_ELSE);
    HashInsert(arr, len, "for", 3, KW_FOR);
    HashInsert(arr, len, "while", 5, KW_WHILE);
    HashInsert(arr, len, "continue", 8, KW_CONTINUE);
    HashInsert(arr, len, "break", 5, KW_BREAK);
    HashInsert(arr, len, "return", 6, KW_RETURN);
    HashInsert(arr, len, "sizeof", 6, KW_SIZEOF);
    return 0;
}

void PrintWithColor(char *s, int len, int token) {
    int color = -1;
    if(token >= TK_PLUS && token <= TK_AND || token == TK_SEMICOLON || token == TK_COMMA) {
        color = COLOR_OPERATOR;
    } else if(token >= TK_OPENPA && token <= TK_END) {
        color = COLOR_SEPARATOR;
    } else if(token == TK_REF) {
        color = COLOR_REF;
    } else if(token == TK_CINT || token == TK_CFLOAT) {
        color = COLOR_NUM;
    } else if(token == TK_CCHAR || token == TK_CSTR) {
        color = COLOR_CHAR;
    } else if(token >= KW_CHAR && token <= KW_SIZEOF) {
        color = COLOR_KEYWORD;
    } else if(token == TK_IDENTV) {
        color = COLOR_IDENTV;
    } else if(token == TK_IDENTF) {
        color = COLOR_IDENTF;
    }
    PrintAtWindows(s, len, color);
}
