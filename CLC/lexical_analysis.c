//
//  lexical_analysis.c
//  CLC
//
//  Created by Albert Zhang on 2022/1/8.
//
//
//  词法分析源文件

#include "compiler.h"

//将字符串分割为单词存储, 并将其打印输出
node* split(char *str, node *anynode) {
    node *lastnode = anynode;
    while(lastnode->next) {
        lastnode = lastnode->next;
    }   //定义一个链表结点指针并使其指向链表末尾结点
    return NULL;
}
