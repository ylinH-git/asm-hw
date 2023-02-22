#include "CBackground.h"
#include <string>
#include <graphics.h>
#include "common_def.h"

CBackground::CBackground()
{
	m_sFile = nullptr;
}

CBackground::CBackground(const LPCTSTR sFile)
{
	SetFile(sFile);
	loadimage(&IMBackground, m_sFile, GAME_WIDTH, GAME_HEIGHT);
	loadimage(&IMBackgroundWithBEAN, m_sFile, GAME_WIDTH, GAME_HEIGHT);
}

void CBackground::SetFile(const LPCTSTR sFile)
{
	m_sFile = sFile;
}

IMAGE* CBackground::GetBG()
{
	return &IMBackground;
}

IMAGE* CBackground::GetBGWBEAN()
{
	return &IMBackgroundWithBEAN;
}

void CBackground::Paint()
{
	SetWorkingImage(&IMBackgroundWithBEAN);
	setlinecolor(BLUE);
	int bias = BLOCK_SIZE / 2;
	for (int i = 0; i < MAP_ROW; i++)
	{
		for (int j = 0; j < MAP_COLUMN; j++)
		{
			if (map[i * MAP_COLUMN + j] == 1)
			{
				setfillcolor(WHITE);
				solidcircle(j * BLOCK_SIZE + bias, i * BLOCK_SIZE + bias, 1);
			}
			else if (map[i * MAP_COLUMN + j] == 2)
			{
				setfillcolor(YELLOW);
				solidcircle(j * BLOCK_SIZE + bias, i * BLOCK_SIZE + bias, 3);
			}
		}
	}
	SetWorkingImage(nullptr);
	putimage(0, 0, &IMBackgroundWithBEAN);
}

bool CBackground::isWall(int nX, int nY)
{
	return false;
}
