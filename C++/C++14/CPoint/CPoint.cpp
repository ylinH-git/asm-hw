#include <iostream>
#include <math.h>
class CPoint
{
public:
    CPoint() :m_x(0), m_y(0){}
    CPoint(double x, double y) :m_x(x), m_y(y)
    {
        SetPoint(x, y);
    }
    void SetPoint(double x, double y)
    {
        m_x = x;
        m_y = y;
    }
    double GetX() const
    {
        return m_x;
    }
    double GetY() const
    {
        return m_y;
    }
    virtual double GetDistance() const
    {
        return sqrt(m_x * m_x + m_y * m_y);
    }
private:
    double m_x;
    double m_y;
};

class CPoint3D: public CPoint
{
public:
    using CPoint::CPoint;
    CPoint3D() :m_z(0) {}
    CPoint3D(double x, double y, double z) :CPoint(x, y), m_z(z){}
    void SetPoint3D(double x, double y, double z)
    {
        SetPoint(x, y);
        m_z = z;
    }
    double GetZ() const
    {
        return m_z;
    }
    double GetDistance() const
    {
        double x = GetX();
        double y = GetY();
        return sqrt(x * x + m_z * m_z);
    }
private:;
    double m_z;
};

double GetDistance(CPoint* point)
{
    return point->GetDistance();
}
int main()
{
    CPoint3D ins;
    ins.SetPoint3D(1, 2, 3);

    CPoint ins2(4, 5);
    std::cout << GetDistance(&ins) << " " << GetDistance(&ins2) << std::endl;
    return 0;
}

