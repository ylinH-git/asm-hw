#include <stdio.h>
#include <stdlib.h>

int funtest()
{
	printf("Hello world\r\n");
	return 0;
}

int main (int argc, char argv[])
{
	int (*pfn)() = NULL;
	char *p;
	int i;
	pfn = funtest;
	p = (char *)*pfn;
	if(p[0] == (char) 0xE9)
	{
		// E9后面的地址 = 目标地址 - 当前地址 - 5
		i = (int)p[1];
		p += i + 5;
	}
	while(p[0] != (char) 0xC0)
	{
		printf("%02hx ", *(short int *)p);
		((short int *)p)++;
	}
	printf("%02hx\r\n", *(short int *)p);
	system("pause");
	return 0;
}