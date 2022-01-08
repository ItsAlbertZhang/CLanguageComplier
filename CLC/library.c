//
//  library.c
//  CLC
//
//  Created by Albert Zhang on 2022/1/8.
//
//
//  基础功能库源文件

#include "compiler.h"

//单词链表的头节点初始化
int NodeInit(node *head) {
    int ret = -1;
    if(head) {
        head->word.token = -1;
        head->word.row = 0;
        head->word.s = NULL;
        head->word.len = 0;
        head->next = NULL;
        ret = 0;
    }
    return ret;
}

//单词链表的结点新增
node* NodeAdd(node *anynode, int token, int row, char *s, int len) {
    //新建结点
    node *newnode = (node*)malloc(sizeof(node));
    newnode->word.token = token;
    newnode->word.row = row;
    newnode->word.s = (char*)malloc(sizeof(char) * (len + 1));
    memcpy(newnode->word.s, s, (unsigned)sizeof(char) * len);
    newnode->word.s[len] = '\0';
    newnode->next = NULL;
    //将新建的结点链接至链表尾
    node *lastnode = anynode;
    while(lastnode->next) {
        lastnode = lastnode->next;
    }
    lastnode->next = newnode;
    //返回新增结点
    return newnode;
}

//单词链表的结点删除
//暂时无需实现

//单词链表的结点值查找(char* node->word->s)
char* NodeGetWord(node *thisnode) {
    return thisnode->word.s;
}

//已知词哈希表的初始化
int HashInit(hashnode **arr, int len) {
    int ret = -1;
    if(arr) {
        for(int i = 0; i < len; i++) {
            arr[i] = NULL;
        }
        ret = 0;
    }
    return ret;
}

//已知词哈希表的查询(S,search)和添加(A,add)
int HashSA(hashnode *arr, int hashtablelen, char *s, int strlen) {
    int ret = -1;
    return ret;
}
















