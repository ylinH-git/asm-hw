#pragma once
#include <graphics.h>
#include <vector>
class CAnimation
{
public:
	CAnimation();

	void Add(IMAGE image);

	IMAGE& GetImage();

	void Update();
private:
	std::vector<IMAGE*> m_imgList;
	int m_nIdx;
};

class CAnimationGroup
{
public:
	CAnimationGroup(int nFlashTime);

	void Add(CAnimation ani);

	void SetIdx(int nIdx);

	IMAGE& GetImage();

	void Update();
private:
	std::vector<CAnimation> m_aniList;
	int m_nIdx;
	int m_nCount;
	int m_nFlashTime;
};