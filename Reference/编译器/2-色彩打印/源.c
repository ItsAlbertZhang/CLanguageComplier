#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);//�ȵõ�һ�����
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);//��ɫ
	printf("This is intensity.\n");
	SetConsoleTextAttribute(h, FOREGROUND_BLUE);
	printf("This is blue.\n");
	SetConsoleTextAttribute(h, FOREGROUND_GREEN);
	printf("This is green.\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED);
	printf("This is red.\n");
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("This is blue and intensity.\n");
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("This is green and intensity.\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("This is red and intensity.\n");
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN);
	printf("This is blue and green.\n");
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_RED);
	printf("This is green and red.\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE);
	printf("This is red and blue.\n");
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("This is blue and green and intensity.\n");
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("This is green and red and intensity.\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("This is red and blue and intensity.\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("This is red and green and blue.\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("This is red and green and blue and intensity.\n");
	// SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);//��ɫ
	// printf("second\n");
	// SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);//��ɫ������
	// printf("third\n");
	// SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);	//��ɫ
	// printf("fourth\n");
	system("pause");
}