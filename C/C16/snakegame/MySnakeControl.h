#include "unicode.h"

#ifndef SNAKECONTROL_H
#define SNAKECONTROL_H

#define ROW 12
#define COL 16
#define WALL 1
#define SNAKE 2
#define FOOD 3
#define TOP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

TCHAR g_szMaxBuff[ROW][COL];
TCHAR g_szSnakeBuff[(ROW - 2) * (COL - 2)][2];

int g_nSnakeX;
int g_nSnakeY;
int g_nFoodX;
int g_nFoodY;
int g_nSnakeCount;
int nDirection;

int StartGame();

#endif
