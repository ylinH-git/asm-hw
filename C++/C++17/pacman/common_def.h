#pragma once
#define BLOCK_SIZE 20
#define PERSON_SIZE 30			// 人物大小 
#define MAP_COLUMN 21			// 地图方格列数
#define MAP_ROW 27				// 地图方格行数
#define MAP_CNT (MAP_COLUMN * MAP_ROW)	// 地图方格总数

#define GAME_WIDTH (MAP_COLUMN * BLOCK_SIZE)	// 游戏窗口宽度
#define GAME_HEIGHT (MAP_ROW * BLOCK_SIZE)	// 游戏窗口高度

#define END_KEY 13
#define TOP_KEY 'w'
#define BOTTOM_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define WALL 3
#define BEAN 1
#define BIGB 2


#define MOVE_TOLERENCE 5
#define EAT_TOLERANCE 8	
#define FIGHT_TOLERANCE 12
