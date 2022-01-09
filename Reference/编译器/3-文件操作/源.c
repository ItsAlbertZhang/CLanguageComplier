#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main()
{
	FILE* fp;
	fp = fopen("file.txt", "r");//打开文件
	if (NULL == fp)
	{
		perror("fopen");//判断刚刚执行失败的函数的失败原因
		return -1;
	}
	//读取文件,一次读一个字符
	//char c;
	//while ((c = fgetc(fp)) != EOF)
	//{
	//	putchar(c);
	//}
	char buf[1000];//一次读一行
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		printf("%s", buf);
	}
	//关闭文件
	fclose(fp);
	return 0;
}