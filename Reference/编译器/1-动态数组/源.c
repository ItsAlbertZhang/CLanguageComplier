#include <stdio.h>

//��̬����ʵ��ԭ��
int main()
{
	int* a = (int*)malloc(sizeof(int) * 5);
	int i;
	for (i = 0; i < 5; i++)
	{
		a[i] = i;
	}
	a = (int*)realloc(a, sizeof(int) * 10);
	return 0;
}