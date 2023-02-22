// encode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>

int main(int argc, char* argv[])
{
	char *str= "Hello";
	int i = 0;
	while(i < strlen(str))
	{
		printf("%c", str[i] ^ i);
		i++;
	}
	getchar();
	return 0;
}

