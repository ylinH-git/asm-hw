#include <vector>
#include <graphics.h>
#include "CAnimation.h"

CAnimation::CAnimation(): m_nIdx(0){}

void CAnimation::Add(IMAGE image)
{
	IMAGE* p = new IMAGE(image);
	m_imgList.push_back(p);
}

IMAGE& CAnimation::GetImage()
{
	return *m_imgList[m_nIdx];
}

void CAnimation::Update()
{
	m_nIdx++;
	if (m_nIdx >= m_imgList.size()) 
	{
		m_nIdx = 0;
	}	
}

CAnimationGroup::CAnimationGroup(int nFlashTime) :m_nIdx(0), m_nCount(0), m_nFlashTime(nFlashTime) {}
void CAnimationGroup::Add(CAnimation ani)
{
	m_aniList.push_back(ani);
}

void CAnimationGroup::SetIdx(int nIdx)
{
	m_nIdx = nIdx;
}

IMAGE& CAnimationGroup::GetImage()
{
	return m_aniList[m_nIdx].GetImage();
}

void CAnimationGroup::Update()
{
	m_nCount++;
	if (m_nCount >= m_nFlashTime)
	{
		m_nCount = 0;
		m_aniList[m_nIdx].Update();
	}
}
