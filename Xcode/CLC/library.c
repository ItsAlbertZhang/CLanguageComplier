//
//  library.c
//  CLC
//  基础功能库源文件
//  Created by Albert Zhang on 2022/1/8.
//

#include "compiler.h"

//单词链表的头节点初始化
int NodeInit(node *head) {
    int ret = -1;
    if(head) {
        head->word.token = -1;
        head->word.row = 0;
        head->word.s = NULL;
        head->next = NULL;
        ret = 0;
    }
    return ret;
}

//单词链表的结点新增
node* NodeAdd(node *anynode, int token, int row, char *s, int strlen) {
    //新建结点
    node *newnode = (node*)malloc(sizeof(node));
    newnode->word.token = token;
    newnode->word.row = row;
    newnode->word.s = (char*)malloc(sizeof(char) * (strlen + 1));
    memcpy(newnode->word.s, s, sizeof(char) * strlen);
    newnode->word.s[strlen] = '\0';
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

//哈希处理
int elf_hash(char* s, int strlen) {
    int h = 0, g = 0;
    for(int i = 0; i < strlen; i++) {
        h = (h << 4) + s[i];
        g = h & 0xf0000000;
        if(g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h % HASH_MAX;
}

//已知词哈希表的初始化
int HashInit(hashnode *arr, int len) {
    int ret = -1;
    if(arr) {
        for(int i = 0; i < len; i++) {
            arr[i].token = 0;
            arr[i].word[0] = 0;
            arr[i].next = NULL;
        }   //初始化哈希表的每个拉链头
        ret = 0;
    }
    return ret;
}

//已知词哈希表的查询(S,search)和添加(A,add)
int HashSA(hashnode *arr, int hashtablelen, char *s, int strlen) {
    int ret = -1;
    int i = elf_hash(s, strlen);    //进行哈希处理
    hashnode *maynode = &arr[i];
    //查询该单词
    while(maynode->next) {
        maynode = maynode->next;
        if(0 == strcmp(maynode->word, s)) { //找到该单词
            ret = maynode->token;   //返回该单词的token编码
            break;
        }
    }
    if(ret < 0) {   //未找到该单词,则进行添加
        hashnode *newnode = (hashnode*)malloc(sizeof(hashnode));
        newnode->token = GetToken(s, strlen);
        memcpy(newnode->word, s, sizeof(char) * strlen);
        newnode->word[strlen] = '\0';
        newnode->next = NULL;
        ret = newnode->token;
    }
    return ret;
}

