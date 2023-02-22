#define ROW 12
#define COL 16
#define WALL 1
#define SNAKE 2
#define FOOD 3
#define TOP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

extern char g_szMaxBuff[ROW][COL];
extern char g_szSnakeBuff[(ROW - 2) * (COL - 2)][2];

extern int g_nSnakeX;
extern int g_nSnakeY;
extern int g_nFoodX;
extern int g_nFoodY;
extern int g_nSnakeCount;
extern int nDirection;


int StartGame();