#include <stdio.h>
#include <stdlib.h>
#include "MySnakeControl.h"

int ShowUI(int res)
{
	int i, j;
	system("cls");
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (g_szMaxBuff[i][j] == WALL)
			{
				printf("■");
			}
			else if (g_szMaxBuff[i][j] == SNAKE)
			{
				printf("◆");
			}
			else if (g_szMaxBuff[i][j] == FOOD)
			{
				printf("◎");
			}
			else
			{
				printf("  ");
			}
		}
		printf("\r\n");
	}
	printf("当前总积分：%d ", g_nSnakeCount - 1);
	printf("nDirection: %d\r\n", nDirection);
	if(res == 1)
	{
		printf("You are win!\r\n");
		printf("可输入空格重新开始游戏，esc退出");
	}
	else if(res == 2)
	{
		printf("GameOver!\r\n");
		printf("可输入空格重新开始游戏，esc退出");
	}
	return 0;
}