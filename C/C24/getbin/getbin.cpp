// getbin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	int i = 23;
	int j = 31;
	printf("all: ", ((unsigned int)i >> 15) % 2 + '0');
	while(j >= 0)
	{
		printf("%c", ((unsigned int)i >> j) % 2 + '0');
		j--;
	}
	printf("\r\n");
	printf("15 :%c\r\n", ((unsigned int)i >> 15) % 2 + '0');
	printf("16 :%c\r\n", ((unsigned int)i >> 16) % 2 + '0');
	getchar();
	return 0;
}

