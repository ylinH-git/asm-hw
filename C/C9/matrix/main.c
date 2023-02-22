#include <stdio.h>
#include <stdlib.h>

void PrintMatrix(int matrix[][21], int size)
{
	int i, j;
	for(i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{	
			printf("%4d", matrix[i][j]);
		}
		printf("\r\n");
	}
}

void FillMatrix(int matrix[][21], int size)
{
	int i = size - 1, j = size / 2, currentNum = 1;
	while (i >= 0 && j >= 0 && currentNum <= size * size)
	{
		matrix[i][j] = currentNum;
		if(matrix[(i + 1) % size][(j + 1) % size] != 0)
		{
			i = i - 1;
		}
		else {
			i = (i + 1) % size;
			j = (j + 1) % size;
		}
		currentNum++;
	}
}

int main (int argc, char *argv[])
{
	int matrix[21][21] = {0};
	int size;
	printf("请输入一个（1-21）的奇数：");
	scanf("%2d", &size);
	while(size <=0 || size > 21 || !(size % 2))
	{
		printf("输入的参数有误，请重新输入！\r\n");
		scanf("%2d", &size);
	}
	FillMatrix(matrix, size);
	PrintMatrix(matrix, size);
	system("pause");
	return 0;
}