// pointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	int i = 0;
	int neg = -1;
	int *pointer = (int *)neg;
	while((unsigned int) pointer !=0 )
	{
		pointer = (int *)((unsigned int) pointer >> 1);
		i++;
	}
	printf("%d", i);
	getchar();
	return 0;
}

