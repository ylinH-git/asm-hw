#pragma once
class CPoint
{
public:
  CPoint(int nX = 0, int nY = 0)
  {
    m_nX = nX;
    m_nY = nY;
  }
  int GetX()const
  {
    return m_nX;
  }
  int GetY()const
  {
    return m_nY;
  }
  void SetX(int nX)
  {
    m_nX = nX;
  }
  void SetY(int nY)
  {
    m_nY = nY;
  }
private:
  int m_nX;
  int m_nY;
};

class CCubePoint:public CPoint
{
public:
  void SetRowCol(int nRow, int nCol)
  {
    SetX(nCol);
    SetY(nRow);
  }
  void GetRowCol(int& nRow, int& nCol)
  {
    nCol = GetX();
    nRow = GetY();
  }
private:
  using CPoint::GetX;
  using CPoint::GetY;
  using CPoint::SetX;
  using CPoint::SetY;
};

