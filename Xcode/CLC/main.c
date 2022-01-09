//
//  main.c
//  CLC
//  main函数文件
//  Created by Albert Zhang on 2022/1/8.
//

#include "compiler.h"

int main(int argc, const char * argv[]) {
    FILE* fp;
    fp = fopen("../code.c", "r");    //打开文件
    if(NULL == fp) {
        perror("fopen");    //如果文件打开失败, 则输出失败原因
        return -1;
    }
    if(fp) {    //如果文件打开成功
        int row = 0;    //行数
        char s[1000]; //定义一个字符数组, 长度为1000
        node *head = (node*)malloc(sizeof(node));   //定义一个单词链表头节点
        NodeInit(head); //初始化单词链表头节点
        node *lastnode = head;
        hashnode hashtable[HASH_MAX];  //定义一个已知词哈希表, 用于快速查询是否已有某单词, 对于已有的单词快速查询其token编码
        //hashtable数组的每个成员都是一个带头结点的链表头(即拉链法的拉链头),表头不存储实际信息,其next指向的才是第一个实际结点.
        HashInit(hashtable, HASH_MAX);  //初始化已知词哈希表
        HashInitCLCkey(hashtable, HASH_MAX);
        //从文件中逐行读取, 每次读取一行, 并将读取到的内容存储至str数组
        while(fgets(s, sizeof(s), fp) != NULL) {
            row++;
            lastnode = split(s, lastnode, hashtable, HASH_MAX, row);
            //fputs(s, stdout);
        }
        printf("This is a breakpoint to Debug.\n");
    }
    fclose(fp); //关闭文件
    return 0;
}
