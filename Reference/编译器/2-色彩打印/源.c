#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);//先得到一个句柄
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);//灰色
	printf("first\n");
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);//绿色
	printf("second\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);//褐色，常量
	printf("third\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);	//红色
	printf("fourth\n");
	system("pause");
}