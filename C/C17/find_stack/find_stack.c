// find_stack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include "string.h"

#define DEBUG_SIZE 16
#define REGISTER_SIZE 3
void stack3 ()
{
	int flag = 1;
	int *p = &flag;
	int *stackTopPoint = &p[- DEBUG_SIZE - 15 - REGISTER_SIZE];
	int i, j = 0;
	int *tempMemoryStackBottom = p + 1;
	char str[32] = {0};
	for(i = 3; i >= 0; i--)
	{
		switch(i)
		{
			case 3:
				strcpy(str, "stack3");
				break;
			case 2:
				strcpy(str, "stack2");
				break;
			case 1:
				strcpy(str, "stack1");
				break;
			case 0:
				strcpy(str, "main");
				break;
		}
		if (i == 2 || i == 1)
		{
			j = 2;
		} 
		else if(i == 0)
		{
			j = 3;
		}
		stackTopPoint++;
		printf("内存地址：%p 值：%p %s寄存器环境\r\n", stackTopPoint, *stackTopPoint, str);
		stackTopPoint++;
		printf("内存地址：%p 值：%p %s寄存器环境\r\n", stackTopPoint, *stackTopPoint, str);
		stackTopPoint++;
		printf("内存地址：%p 值：%p %s寄存器环境\r\n", stackTopPoint, *stackTopPoint, str);
		
		do 
		{
			stackTopPoint++;
			printf("内存地址：%p 值：%p %s局部变量空间\r\n", stackTopPoint, *stackTopPoint, str);
		} while(stackTopPoint < tempMemoryStackBottom - 1);
		
		
		stackTopPoint++;
		printf("内存地址：%p 值：%p %s调用函数栈底\r\n", stackTopPoint, *stackTopPoint, str);
		stackTopPoint++;
		printf("内存地址：%p 值：%p %s返回地址\r\n", stackTopPoint, *stackTopPoint, str);

		while(j > 0)
		{
			stackTopPoint++;
			printf("内存地址：%p 值：%p %s参数\r\n", stackTopPoint, *stackTopPoint, str);
			j--;
		}
		if (i == 2 || i == 1)
		{
			j = 2;
		} 
		else if (i == 0)
		{
			j = 3;
		}
		tempMemoryStackBottom = *(stackTopPoint - j - 1);
	}

	system("pause");
}

float stack2 (int x, int y)
{
	float pi = 3.1415f;
	stack3();
	return (x + y) * pi;
}

int stack1 (int x, int y)
{
	float xy = stack2(x, y);
	return x + y + (int) xy;
}

int main(int argc, char* argv[])
{
	int x = 10, y = 20;
	stack1(x, y);
	return 0;
}

