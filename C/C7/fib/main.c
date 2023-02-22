#include <stdio.h>
#include <stdlib.h>


unsigned int fib(int n)
{
	if (n < 3)
	{
		return 1;
	}
	return fib(n - 1) + fib(n - 2); 
}
int main(int argc, char *argv[])
{
	int i;
	for(i = 1; i <= 20; i++)
	{
		printf("%u\r\n", fib(i));
	}
	
	system("pause");
}