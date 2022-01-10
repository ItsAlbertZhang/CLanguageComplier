//
//  print.c
//  CLC
//
//  Created by Albert Zhang on 2022/1/9.
//

#include "compiler.h"
#include <Windows.h>

void PrintAtWindows(char *s, int len, int color) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	switch(color) {
	case COLOR_OPERATOR:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case COLOR_SEPARATOR:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case COLOR_REF:
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
		break;
	case COLOR_NUM:
		SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case COLOR_CHAR:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case COLOR_KEYWORD:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case COLOR_IDENTV:
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case COLOR_IDENTF:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	}
	for(int i = 0; i < len; i++) {
		if(s[i] == '\t') {
			printf("    ");	//制表符可能为8个字符,因此此处用4个空格代替
		} else {
			putchar(s[i]);
		}
	}
}