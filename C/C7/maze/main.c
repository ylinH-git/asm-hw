#include <stdio.h>
#include <stdlib.h>

#define MAZACOL sizeof("111111111111111111111111111111111111")
void ShowMaze(char maza[][MAZACOL], int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		puts(maza[i]);
	}
}

void GoStep(char maza[][MAZACOL], char x, char y, int size)
{
		if(maza[x][y] == '$')
		{
			system("cls");
			ShowMaze(maza, size);
			printf("ÓÎÏ·Í¨¹Ø\n");
			system("pause");
			exit(0);
		}

		system("cls");
		maza[x][y] = '\2';
		ShowMaze(maza, size);
		system("pause");
		if(maza[x][y - 1] == '0' || maza[x][y - 1] == '$')
		{
			GoStep(maza, x, y - 1, size);
		}

		if(maza[x][y + 1] == '0' || maza[x][y + 1] == '$')
		{
			GoStep(maza, x, y + 1, size);
		}

		if(maza[x + 1][y] == '0' || maza[x + 1][y] == '$')
		{
			GoStep(maza, x + 1, y, size);
		}


		if(maza[x - 1][y] == '0' || maza[x - 1][y] == '$')
		{
			GoStep(maza, x - 1, y, size);
		}

		maza[x][y] = '\1';
		system("cls");
		ShowMaze(maza, size);
		system("pause");
}
int main() {
	char maza[][MAZACOL] = {
		"111111111111111111111111111111111111",
		"100000000111111110000000011111111111",
		"111111101111111110111111011111111111",
		"111111100001111110111111000000001111",
		"100000001100111110111111011111101111",
		"111111101110111110111111011111101111",
		"111111101110111110111111011111101111",
		"111111101110111110111111011111101111",
		"111111111110000000000000000000000001",
		"111111111111111010111101111111111101",
		"111111111000001000111101111111111101",
		"111111111111100010011101111111111101",
		"1111111111111$1111000000000000000001",
		"111111111111111111111111111111111111",
	};

	GoStep(maza , 1, 1, sizeof(maza) / sizeof(maza[0]));
	system("pause");
	return 0;
}