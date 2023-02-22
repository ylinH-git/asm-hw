#include <stdio.h>
#include <stdlib.h>

int prime(int x)
{
	static i = 2;
	if(x <= 1)
	{
		return 0;
	}
	if(x == 2 || x == 3)
	{
		return 1;
	}
	
	if(i < x && x % i)
	{
		i++;
		return prime(x);
	}

	if(i == x) 
	{
		return 0;
	}
	return 1;
}

int main (int argc, char *argv[])
{
	int num;
	printf("请输入一个正整数：");
	scanf("%d", &num);
	printf("%d", prime(num));
	system("pause");
	return 0;
}
