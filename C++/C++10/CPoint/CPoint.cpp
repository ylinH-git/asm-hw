#include <iostream>
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
private:
    double m_x;
    double m_y;
};

class CPoint3D: public CPoint
{
public:
    using CPoint::CPoint;
    CPoint3D() :m_z(0) {}
    CPoint3D(double x, double y, double z) :m_z(z)
    {
        SetPoint3D(x, y, z);
    }
    void SetPoint3D(double x, double y, double z)
    {
        SetPoint(x, y);
        m_z = z;
    }
    double GetZ() const
    {
        return m_z;
    }
private:;
    double m_z;
};

int main()
{
    CPoint3D ins;
    ins.SetPoint3D(1, 2, 3);

    CPoint3D ins2(4, 5, 6);
    std::cout << ins.GetX() << " " << ins.GetY() << " " << ins.GetZ();
    return 0;
}

