#pragma once
#include "CAnimation.h"
#include "CBackground.h"
#include "CGraph.h"

class CPosition: public CRect
{
public:
	CPosition(int nX, int nY);
	int Left();
	int Top();
	int Right();
	int Bottom();
	CPoint Center();
	void SetPosition(int nX, int nY);
private:
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
};
class CRole
{
public:
	CRole();
	void SetImage(const LPCTSTR pFile, int nDirection, int nState);
	void Paint();
	void SetPosition(int x, int y);
	virtual void Update(int cmd);
	void Clear(IMAGE* bg);
	void Move();
	void SetDir(int nDir);
	void GetNext(int& nDX, int& nDY, int& nI, int& nJ);
	void Go();
	bool IsWall(int nI, int nJ);
	void NextPos(int& nDX, int& nDY, int& nI, int& nJ);
	bool CanTurn(int nDir);
	CPosition GetPosition();
private:
	CAnimationGroup m_aniFaces;
	IMAGE m_image;
	CPosition m_pos;
	int m_nDirection;
};

class CPacman: public CRole
{
public:
	CPacman();
	int GetScore();
	void Eat();
private:
	int m_nScore;
};

class CMonster : public CRole
{
public:
	void Update();
	bool Eat(CPacman& pacman);
};