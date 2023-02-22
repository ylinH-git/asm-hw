#include <stdio.h>
#include <stdlib.h>


// ��ֵ����
int Search(int *arr, int size, int key)
{
	int lo = 0, hi = size - 1, mid;
	while(lo < hi)
	{
		mid = lo + (hi - lo) * (key - arr[lo]) / (arr[hi] - arr[lo]);
		if(arr[mid] < key)
		{
			lo = mid + 1;
		}
		else if (arr[mid] > key)
		{
			hi = mid;
		}
		else
		{
			return mid;
		}
	}
	return lo;
	
}

int main (int argc, int *argv[])
{
	int arr[54] = {0};
	int i, searchKey, searchVal;
	for(i = 0; i < 54; i++)
	{
		arr[i] = i + 1;
	}
	printf("������һ����Ҫ���ҵĺ��루1-54����");
	scanf("%d", &searchKey);
	while(searchKey < 1 || searchKey > 54)
	{
		printf("�������, ���������룺");
		scanf("%2d", &searchKey);
	}
	searchVal = Search(arr, 54, searchKey);
	printf("arr[%d]: %d\r\n", searchVal, arr[searchVal]);

	system("pause");
	return 0;
}