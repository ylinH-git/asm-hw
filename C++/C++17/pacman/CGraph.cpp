#include "CGraph.h"
CPoint::CPoint(int x, int y) :m_nX(x), m_nY(y)
{
    SetPoint(x, y);
}

int CPoint::GetX() const
{
    return m_nX;
}

int CPoint::GetY() const
{
    return m_nY;
}

void CPoint::SetPoint(int nX, int nY)
{
    m_nX = nX;
    m_nY = nY;
}

CRect::CRect(int nX, int nY, int nWidth, int nHeight): m_point(nX, nY), m_nWidth(nWidth), m_nHeight(nHeight)
{
}

void CRect::SetPoint(int nX, int nY)
{
    m_point.SetPoint(nX, nY);
}