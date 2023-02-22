#include <stdio.h>
#include <stdlib.h>

int factorial(int n)
{
	if(n > 1)
	{
		return n * factorial(n-1);
	}
	return 1;
}

int main (int argc, char *argv[])
{
	int re = factorial(10) / (factorial(3) * factorial(7));
	printf("%d", re);
	system("pause");
	return 0;
}