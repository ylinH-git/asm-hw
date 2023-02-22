#include "CAnimation.h"
#include "CGraph.h"
#include "CRole.h"
#include "CBackground.h"
#include "common_def.h"
#include <math.h>

CPosition::CPosition(int nX, int nY): CRect(nX, nY, PERSON_SIZE, PERSON_SIZE)
{
	m_nLeft = m_point.GetX();
	m_nTop = m_point.GetY();
	m_nRight = m_point.GetX() + PERSON_SIZE;
	m_nBottom = m_point.GetY() + PERSON_SIZE;
}

int CPosition::Left()
{
	m_nLeft = m_point.GetX();
	return m_nLeft;
}

int CPosition::Top()
{
	m_nTop = m_point.GetY();
	return m_nTop;
}

int CPosition::Right()
{
	m_nRight = m_point.GetX() + PERSON_SIZE;
	return m_nRight;
}

int CPosition::Bottom()
{
	m_nBottom = m_point.GetY() + PERSON_SIZE;
	return m_nBottom;
}

CPoint CPosition::Center()
{
	return CPoint(m_point.GetX() + PERSON_SIZE / 2, m_point.GetY() + PERSON_SIZE / 2);
}

void CPosition::SetPosition(int nX, int nY)
{
	SetPoint(nX, nY);
}

CRole::CRole():m_pos(0, 0), m_aniFaces(5), m_nDirection(NULL)
{}

void CRole::SetImage(const LPCTSTR pFile, int nDirection, int nState)
{
	loadimage(&m_image, pFile, PERSON_SIZE * nState, PERSON_SIZE * nDirection, 1);
	SetWorkingImage(&m_image);
	for (int i = 0; i < nDirection; i++)
	{
		CAnimation ani;
		for (int j = 0; j < nState; j++)
		{
			IMAGE face;
			int nX = i * PERSON_SIZE;
			int nY = j * PERSON_SIZE;
			getimage(&face, nY, nX, PERSON_SIZE, PERSON_SIZE);
			ani.Add(face);
		}
		m_aniFaces.Add(ani);
	}
	SetWorkingImage(nullptr);
}

void CRole::Paint()
{
	putimage(m_pos.Left() + BLOCK_SIZE / 2, m_pos.Top() + BLOCK_SIZE / 2, &m_aniFaces.GetImage(), SRCINVERT);
}

void CRole::SetPosition(int nX, int nY)
{
	m_pos.SetPoint(nX, nY);
}

void CRole::Update(int cmd)
{
	m_aniFaces.Update();
	if(CanTurn(cmd))
	{
		SetDir(cmd);
	}
	Go();
}

void CRole::Clear(IMAGE* bg)
{
	IMAGE emptyBlock;
	SetWorkingImage(bg);
	getimage(&emptyBlock, m_pos.Left() + BLOCK_SIZE / 2, m_pos.Top() + BLOCK_SIZE / 2, PERSON_SIZE, PERSON_SIZE);

	SetWorkingImage(NULL);
	putimage(m_pos.Left() + BLOCK_SIZE / 2, m_pos.Top() + BLOCK_SIZE / 2, &emptyBlock);
}

void CRole::Move()
{
	switch (m_nDirection)
	{
	case TOP_KEY:
		m_pos.SetPosition(m_pos.Left(), m_pos.Top() - 1);
		break;
	case RIGHT_KEY:
		m_pos.SetPosition(m_pos.Left() + 1, m_pos.Top());
		break;
	case BOTTOM_KEY:
		m_pos.SetPosition(m_pos.Left(), m_pos.Top() + 1);
		break;
	case LEFT_KEY:
		m_pos.SetPosition(m_pos.Left() - 1, m_pos.Top());
		break;
	}
}

void CRole::SetDir(int nDir)
{
	m_nDirection = nDir;
	switch (m_nDirection)
	{
	case TOP_KEY:
		m_aniFaces.SetIdx(2);
		break;
	case RIGHT_KEY:
		m_aniFaces.SetIdx(0);
		break;
	case BOTTOM_KEY:
		m_aniFaces.SetIdx(3);
		break;
	case LEFT_KEY:
		m_aniFaces.SetIdx(1);
		break;
	}
}

void CRole::GetNext(int& nDX, int& nDY, int& nI, int& nJ)
{
	CPoint center = m_pos.Center();

	// 距离最靠近的格点的距离
	nDX = (center.GetX() + BLOCK_SIZE / 2) % BLOCK_SIZE - BLOCK_SIZE / 2;
	nDY = (center.GetY() + BLOCK_SIZE / 2) % BLOCK_SIZE - BLOCK_SIZE / 2;

	int nX = center.GetX() - nDX;
	int nY = center.GetY() - nDY;

	nI = nY / BLOCK_SIZE;
	nJ = nX / BLOCK_SIZE;
}

void CRole::Go()
{
	int nDX, nDY, nI, nJ;
	GetNext(nDX, nDY, nI, nJ);
	NextPos(nDX, nDY, nI, nJ);
	if (nJ < 0 || nJ >= MAP_COLUMN)
	{
		Move();

		if (m_pos.Left() < 0)
			m_pos.SetPosition(m_pos.Left() + GAME_WIDTH, m_pos.Top());
		else if (m_pos.Left() >= GAME_WIDTH)
			m_pos.SetPosition(m_pos.Left() - GAME_WIDTH, m_pos.Top());

		return;
	}

	if(!IsWall(nI, nJ))

	Move();

	return;

}

bool CRole::IsWall(int nI, int nJ)
{
	if (map[nI * MAP_COLUMN + nJ] == WALL)
	{
		const int nThick = BLOCK_SIZE - PERSON_SIZE / 2;	// 墙壁的厚度

		int nDist = NULL;

		switch (m_nDirection)
		{
		case BOTTOM_KEY:
			nDist = (nI * BLOCK_SIZE - nThick) - (m_pos.Top() + PERSON_SIZE);
			break;
		case RIGHT_KEY:
			nDist = (nJ * BLOCK_SIZE - nThick) - (m_pos.Left() + PERSON_SIZE);
			break;
		case TOP_KEY:
			nDist = m_pos.Top() - (nI * BLOCK_SIZE + nThick);
			break;
		case LEFT_KEY:
			nDist = m_pos.Left() - (nJ * BLOCK_SIZE + nThick);
			break;
		}


		if (nDist <= 0)
		{
			return true;
		}
	}
	return false;
}

void CRole::NextPos(int& nDX, int& nDY, int& nI, int& nJ)
{
	switch (m_nDirection)
	{
	case BOTTOM_KEY:
		if (nDX >= 0) nI++;
		break;
	case RIGHT_KEY:
		if (nDY >= 0) nJ++;
		break;
	case TOP_KEY:
		if (nDX <= 0) nI--;
		break;
	case LEFT_KEY:
		if (nDY <= 0) nJ--;
		break;
	}
}

bool CRole::CanTurn(int nDir)
{
	if (nDir == NULL)
	{
		return false;
	}
	else if (nDir != m_nDirection)
	{
		// 转弯限制
		const int nT = MOVE_TOLERENCE;	// 宽容门限

		int nDX, nDY, nI, nJ;
		GetNext(nDX, nDY, nI, nJ);

		bool bFlag = false;

		if (((nDir == RIGHT_KEY || nDir == LEFT_KEY)&& abs(nDX) < nT) || ((nDir == TOP_KEY || nDir == BOTTOM_KEY) && abs(nDY) < nT))
			bFlag = true;

		if (bFlag)		// 是否在转弯容限内
		{
			// 下一个格点
			switch (nDir)
			{
			case BOTTOM_KEY:  nI++;  break;
			case TOP_KEY:	nI--;  break;
			case RIGHT_KEY: nJ++;  break;
			case LEFT_KEY:	nJ--;  break;
			}

			bool nedgeFlag = false;
			if (nJ < 0 || nJ >= MAP_COLUMN)
				nedgeFlag = 1;

			if (!nedgeFlag)
			{
				bool bIsWall = false;

				if (map[nI * MAP_COLUMN + nJ] == WALL)
					bIsWall = true;

				if (!bIsWall)
				{
					m_pos.SetPosition(m_pos.Left() - nDX, m_pos.Top() - nDY);
					return true;
				}
				else
				{
					return false;
				}

			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}

	}
	else if(nDir == m_nDirection)
	{
		return true;
	}
}

CPosition CRole::GetPosition()
{
	return m_pos;
}

CPacman::CPacman(): CRole(), m_nScore(0)
{
}

int CPacman::GetScore()
{
	return m_nScore;
}

void CPacman::Eat()
{
	const int nT = EAT_TOLERANCE;

	// 节点位置
	int nDX, nDY, nI, nJ;
	GetNext(nDX, nDY, nI, nJ);

	if (abs(nDX) + abs(nDY) < nT)
	{
		int nPlace = map[nI * MAP_COLUMN + nJ];
		int nGoal = 0;
		if (nPlace == BEAN)
		{
			nGoal = BEAN;
		}
		else if (nPlace == BIGB)
		{
			nGoal = BIGB;
		}

		if (nGoal != 0)
		{
			map[nI * MAP_COLUMN + nJ] = 0;

			m_nScore += nGoal;
		}
	}
}

void CMonster::Update()
{
	// 节点位置
	int nDX, nDY, nI, nJ;
	GetNext(nDX, nDY, nI, nJ);
	NextPos(nDX, nDY, nI, nJ);
	if(IsWall(nI, nJ))
	{
		int arrCmdList[4] = { TOP_KEY, BOTTOM_KEY, LEFT_KEY, RIGHT_KEY };
		int nR = rand() % 4;
		if (CanTurn(arrCmdList[nR])) {
			SetDir(arrCmdList[nR]);
		}
	}
	Go();
}

bool CMonster::Eat(CPacman& pacman)
{
	int nDx, nDy, nI, nJ;
	GetNext(nDx, nDy, nI, nJ);

	CPosition pos = pacman.GetPosition();
	CPosition myPos = GetPosition();
	int nDist = abs(pos.Left() - myPos.Left()) + abs(pos.Top() - myPos.Top());
	if (nDist <= FIGHT_TOLERANCE)
	{
		return true;
	}

	return false;
}
