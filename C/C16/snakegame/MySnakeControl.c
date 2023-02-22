#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <assert.h>
#include "MySnakeShowUI.h"
#include "MySnakeControl.h"
#include "unicode.h"

TCHAR g_szMaxBuff[ROW][COL] = {0};
TCHAR g_szSnakeBuff[(ROW - 2) * (COL - 2)][2] = {0};

int g_nSnakeX = 0;
int g_nSnakeY = 0;
int g_nFoodX = 0;
int g_nFoodY = 0;
int g_nSnakeCount = 0;
int nDirection = 0;
int lastDirection = 0;

int InitGame()
{
	int i, j;

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (i == 0 || i == ROW - 1 || j == 0 || j == COL -1)
			{
				g_szMaxBuff[i][j] = WALL;
			} else 
			{
				g_szMaxBuff[i][j] = 0;
			}
		}
	}
	return 0;
}

int InitSnake()
{
	g_nSnakeX = rand() % (ROW - 2) + 1;
	g_nSnakeY = rand() % (COL - 2) + 1;

	g_szSnakeBuff[0][0] = g_nSnakeX;
	g_szSnakeBuff[0][1] = g_nSnakeY;

	g_szMaxBuff[g_nSnakeX][g_nSnakeY] = SNAKE;
	g_nSnakeCount++;
	return 0;	
}

int InitFood()
{
	int i = 0;
	g_nFoodX = rand() % (ROW - 2) + 1;
	g_nFoodY = rand() % (COL - 2) + 1;

	// 判断食物初始化坐标是否和蛇重叠
	while(i < g_nSnakeCount)
	{
		if(g_szSnakeBuff[i][0] == g_nFoodX && g_szSnakeBuff[i][1] == g_nFoodY)
		{
			g_nFoodX = rand() % (ROW - 2) + 1;
			g_nFoodY = rand() % (COL - 2) + 1;
			i = 0;
		}
		i++;
	}

	g_szMaxBuff[g_nFoodX][g_nFoodY] = FOOD;
	return 0;
}

int IsMove(int nSnakeX, int nSnakeY)
{
	return g_szMaxBuff[nSnakeX][nSnakeY];
}

int ClearMaxBuff()
{
	int nSnakeX = 0;
	int nSnakeY = 0;
	nSnakeX = g_szSnakeBuff[g_nSnakeCount - 1][0];
	nSnakeY = g_szSnakeBuff[g_nSnakeCount - 1][1];
	g_szMaxBuff[nSnakeX][nSnakeY] = 0;

	return 0;
}

int AddSnake()
{
	int i;
	int nSnakeX = 0;
	int nSnakeY = 0;

	for(i = g_nSnakeCount - 1; i >= 0; i--)
	{
		g_szSnakeBuff[i + 1][0] = g_szSnakeBuff[i][0];
		g_szSnakeBuff[i + 1][1] = g_szSnakeBuff[i][1];
	}
	g_szSnakeBuff[0][0] = g_nFoodX;
	g_szSnakeBuff[0][1] = g_nFoodY;
	g_nSnakeX = g_nFoodX;
	g_nSnakeY = g_nFoodY;
	g_nSnakeCount++;

	for(i = 0; i < g_nSnakeCount; i++)
	{
		nSnakeX = g_szSnakeBuff[i][0];
		nSnakeY = g_szSnakeBuff[i][1];
		g_szMaxBuff[nSnakeX][nSnakeY] = SNAKE;
	}
	return 0;
}

int UpdateSnakeBuffAndMaxBuff()
{
	int i;
	int nSnakeX = 0;
	int nSnakeY = 0;

	ClearMaxBuff();
	// 更新蛇身
	for(i = g_nSnakeCount - 1; i > 0; i--)
	{
		g_szSnakeBuff[i][0] = g_szSnakeBuff[i - 1][0];
		g_szSnakeBuff[i][1] = g_szSnakeBuff[i - 1][1];
	}
	
	// 更新蛇头
	g_szSnakeBuff[0][0] = g_nSnakeX;
	g_szSnakeBuff[0][1] = g_nSnakeY;

	// 更新到游戏数组
	for(i = 0; i < g_nSnakeCount; i++)
	{
		nSnakeX = g_szSnakeBuff[i][0];
		nSnakeY = g_szSnakeBuff[i][1];
		g_szMaxBuff[nSnakeX][nSnakeY] = SNAKE;
	}
	return 0;
}

int RefreshSnake()
{
	int i;
	for(i = 0; i < (ROW - 2) * (COL - 2); i++)
	{
		g_szSnakeBuff[i][0] = 0;
		g_szSnakeBuff[i][1] = 0;
	}
	g_nSnakeCount = 0;
	InitSnake();
	return 0;
}

int MoveAction() 
{
	int nRet = 0;
	int result = 0;
	int nextPosition[2];
	if(nDirection == TOP || nDirection == DOWN)
	{
		nextPosition[0] = nDirection == TOP ? g_nSnakeX - 1 : g_nSnakeX + 1;
		nextPosition[1] = g_nSnakeY;
	}
	else if(nDirection == LEFT || nDirection == RIGHT)
	{
		nextPosition[0] = g_nSnakeX;
		nextPosition[1] = nDirection == LEFT ? g_nSnakeY - 1 : g_nSnakeY + 1;
	}
	nRet = IsMove(nextPosition[0], nextPosition[1]);
	if(nRet == 0)
	{
		switch(nDirection)
		{
			case TOP: 
				g_nSnakeX--;
				break;
			case DOWN: 
				g_nSnakeX++;
				break;
			case LEFT:
				g_nSnakeY--;
				break;
			case RIGHT: 
				g_nSnakeY++;
				break;
		}
		UpdateSnakeBuffAndMaxBuff();
	}
	else if (nRet == FOOD)
	{
		AddSnake();
		InitFood();
		// （选做）AI玩法，自动控制方向吃食物，不死
	}
	else if (nRet == WALL || nRet == SNAKE)
	{
		// 判断是否蛇已无法前进
		if(g_nSnakeCount == (ROW - 2) * (COL - 2) - 1)
		{
			result = 1;
		}
		else
		{
			result = 2;
		}
	}
	ShowUI(result);
	return result;
}

int ControlSnake()
{
	int nKey = 0;
	int nRet = 0;
	int speed = 500;
	clock_t clockBeginTime = clock();
	clock_t clockEndTime =0;
	while (1)
	{
		clockEndTime = clock();
		speed = 500 - ((g_nSnakeCount - 1) / 5) * 100;
		speed = speed <= 60 ? 60 : speed;
		if(clockEndTime - clockBeginTime >= speed)
		{
			clockBeginTime = clockEndTime;
			// 响应动作时如果方向相反则保持与原方向一致
			if(
				(lastDirection == TOP && nDirection == DOWN)
				|| (lastDirection == DOWN && nDirection == TOP)
				|| (lastDirection == LEFT && nDirection == RIGHT)
				|| (lastDirection == RIGHT && nDirection == LEFT)
				)
			{
				nDirection = lastDirection;
			}
			if(
				nDirection == TOP 
				|| nDirection == DOWN 
				|| nDirection == LEFT 
				|| nDirection == RIGHT
				)
			{
				nRet = MoveAction();
				if(nRet != 0)
				{
					nDirection = 0;
				}
				lastDirection = nDirection;
			}
		}
		if(kbhit())
		{
			nKey = _getch(); //非标 慎用
			if(nRet != 0)
			{
				if(nKey == 27)
				{
					exit(0);
				}
				else if(nKey == 32)
				{
					nRet = 0;
					InitGame();
					RefreshSnake();
					InitFood();
					ShowUI(0);
				}
				continue;
			}
			if (nKey == TOP || nKey == DOWN || nKey == LEFT || nKey == RIGHT)
			{
				nDirection = nKey;
			}
		}
	}
	return nRet;
}

int StartGame()
{
	int nRet = 0;
	srand((unsigned) time (NULL));
	// InitGame
	InitGame();

	// InitSnake
	nRet = InitSnake();
	assert(nRet == 0);
	// InitFood
	nRet = InitFood();
	assert(nRet == 0);
	// ShowUI
	nRet = ShowUI(0);
	assert(nRet == 0);
	// ControlSnake
	nRet = ControlSnake();
	assert(nRet == 0);
	return nRet;
}
