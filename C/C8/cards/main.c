#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void PrintfCards(int cards[54], int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		printf("%d ", cards[i]);
	}
	printf("\r\n");
}

// ���ֲ���
int Search(int cards[54], int size, int key)
{
	int lo = 0, hi = size - 1, mid;
	while(lo < hi)
	{
		mid = (lo + hi) >> 1;
		if(cards[mid] < key)
		{
			lo = mid + 1;
		}
		else
		{
			hi = mid;
		}
	}
	return lo;
	
}

// ð������
void Sort(int cards[54], int size)
{
	int temp, i, j;
	for(i = 0; i < size; i++) {
		for(j = i; j < size; j++) {
			if(cards[i] > cards[j]) {
				temp = cards[i];
				cards[i] = cards[j];
				cards[j] = temp;
			}
		}
  }
}

// ���ϴ��
void Shuffle(int cards[54], int size)
{
	int i = 0;
	int temp;
	int currentRand;
	srand((int)time(NULL));
	while(i < size)
	{
		currentRand = rand() % (size - i) + i;
		if(currentRand >= i)
		{
			temp = cards[i];
			cards[i] = cards[currentRand];
			cards[currentRand] = temp;
			i++;
		}
		else
		{
			currentRand = rand() % size;
		}
	}
}
int main (int argc, char *argv[])
{
	int cards[54] = {0};
	int i, searchKey, searchVal;
	for(i = 0; i < 54; i++)
	{
		cards[i] = i + 1;
	}

	Shuffle(cards, 54);
	printf("ϴ�ƺ�\r\n");
	PrintfCards(cards, 54);

	printf("�����\r\n");
	Sort(cards, 54);
	PrintfCards(cards, 54);

	printf("������һ����Ҫ���ҵĿ��ţ�1-54����");
	scanf("%d", &searchKey);
	searchVal = Search(cards, 54, searchKey);
	printf("cards[%d]: %d\r\n", searchVal, cards[searchVal]);

	system("pause");
	return 0;
}