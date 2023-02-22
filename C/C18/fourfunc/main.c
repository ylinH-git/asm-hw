#include <stdio.h>
#include <stdlib.h>


void func0 () 
{
	printf("hello\r\n");
}

void func1 () 
{
	printf("world\r\n");
}

void func2 () 
{
	printf("c++\r\n");
}

void func3 () 
{
	printf("asm\r\n");
}


int main (int argc, char argv[])
{

	void (*p)() = NULL;
	int i;
	printf("num: ");
	scanf("%d", &i);

	switch(i)
	{
		case 0:
			p = func0;
			break;
		case 1:
			p = func1;
			break;
		case 2:
			p = func2;
			break;
		case 3:
			p = func3;
			break;
		default:
			printf("³ö´í");
			return -1;
	}
	p();
	system("pause");
	return 0;
}