#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 10000

int isPrime (int n)
{
	int i;
	if(n == 1)
	{
		return 0;
	}
	for(i = n - 1; i > 1; i--)
	{
		if(!(n % i)) 
		{
			return 0;
		}
	}
	return 1;
}
void Filter(int *arr, int size)
{
	int m = (int)sqrt(size), i, j;
	if( m * m == size)
	{
		Filter(arr, m);
		for(i = m; i < size; i++)
		{
			for(j = 0; j < m; j++)
			{
				if(arr[j] != 0 && !(arr[i] % arr[j]))
				{
					arr[i] = 0;
				}
			}
		}
	}
	else {
		for(i = 0; i < size; i++)
		{
			if(!isPrime(arr[i]))
			{
				arr[i] = 0;
			}
		}
	}
}
int main ()
{
	int arr[SIZE] = {0};
	int i;
	int count = 0;
	for(i = 0; i < SIZE; i++)
	{
		arr[i] = i + 1;
	}
	Filter(arr, SIZE);
	for(i = 0; i < SIZE; i++)
	{
		if(arr[i] != 0)
		{
			printf("%4d,", arr[i]);
			count++;
			if(!(count % 5))
			{
				printf("\r\n");
			}
		}
	}
	printf("\r\ntotal:%d \r\n", count);
	system("pause");
	return 0;
}