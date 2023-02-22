#pragma once
#include "CRole.h"
#include "CBackground.h"
class CGame
{
public:
	CGame();
	void Init();
	void Close();
	void Run(int cmd);
private:
	CPacman m_pacman;
	CMonster m_arrMonster[4];
	CBackground m_bg;
};