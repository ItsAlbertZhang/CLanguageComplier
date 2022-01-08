//
//  main.c
//  CLC
//
//  Created by Albert Zhang on 2022/1/8.
//
//
//  main函数文件

#include "compiler.h"

int main(int argc, const char * argv[]) {
    FILE* fp;
    fp = fopen("../code.c", "r");    //打开文件
    if(NULL == fp) {
        perror("fopen");    //如果文件打开失败, 则输出失败原因
        return -1;
    }
    if(fp) {    //如果文件打开成功
        int row = 1;    //行数从1开始
        char str[1000]; //定义一个字符数组, 长度为1000
        node *head = (node*)malloc(sizeof(node));   //定义一个单词链表头节点
        NodeInit(head); //初始化单词链表头节点
        hashnode* hashtable[1000];  //定义一个已知词哈希表, 用于快速查询是否已有某单词, 对于已有的单词快速查询其token编码
        HashInit(hashtable, sizeof(hashtable) / sizeof(hashtable[0]));  //初始化已知词哈希表
        //从文件中逐行读取, 每次读取一行, 并将读取到的内容存储至str数组
        while(fgets(str, sizeof(str), fp) != NULL) {
            puts(str);
        }
    }
    //code
    fclose(fp); //关闭文件
    return 0;
}



