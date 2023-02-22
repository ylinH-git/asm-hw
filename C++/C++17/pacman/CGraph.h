#pragma once
#include <math.h>
class CPoint
{
public:
    CPoint(int x, int y);
    int GetX() const;
    int GetY() const;
    void SetPoint(int nX, int nY);
private:
    int m_nX;
    int m_nY;
};

class CRect
{
public:
    CRect(int nX, int nY, int nWidth, int nHeight);
    void SetPoint(int nX, int nY);
protected:
    CPoint m_point;
    int m_nWidth;
    int m_nHeight;
};