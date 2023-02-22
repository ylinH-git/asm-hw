#pragma once
#define BLOCK_SIZE 20
#define PERSON_SIZE 30			// �����С 
#define MAP_COLUMN 21			// ��ͼ��������
#define MAP_ROW 27				// ��ͼ��������
#define MAP_CNT (MAP_COLUMN * MAP_ROW)	// ��ͼ��������

#define GAME_WIDTH (MAP_COLUMN * BLOCK_SIZE)	// ��Ϸ���ڿ��
#define GAME_HEIGHT (MAP_ROW * BLOCK_SIZE)	// ��Ϸ���ڸ߶�

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
