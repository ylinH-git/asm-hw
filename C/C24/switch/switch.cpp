// switch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	int x = 6;
	int y = 9;
	printf("����ǰ��%d %d", x, y);
	x = x ^ y;
	y = x ^ y;
	x = x ^ y;
	printf("������%d %d", x, y);
	getchar();
	return 0;
}

