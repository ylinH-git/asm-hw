#include <iostream>
#define PI 3.1415
using namespace std;
class CPoint
{
public:
    CPoint() :m_x(0), m_y(0) {}
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

class CGraph
{
public:
    CGraph() :m_start(0, 0), m_end(0,0) {}
    CGraph(CPoint& start, CPoint& end)
    {
        SetStartPoint(start);
        SetEndPoint(end);
    }
    void SetStartPoint(CPoint& start)
    {
        m_start = start;
    }
    void SetEndPoint(CPoint& end)
    {
        m_start = end;
    }
    CPoint GetStartPoint()
    {
        return m_start;
    }
    CPoint GetEndPoint()
    {
        return m_end;
    }
private:
    CPoint m_start;
    CPoint m_end;
};

class CCycle : public CGraph
{
public:
    using CGraph::CGraph;
    CCycle() :m_radius(0) {}
    CCycle(double radius) {
        SetRadius(radius);
    }
    void SetRadius(double radius)
    {
        m_radius = radius;
    }
    double GetRadius()
    {
        return m_radius;
    }
    double GetArea()
    {
        return m_radius * m_radius * 3.1415;
    }
private:
    double m_radius;
};

class CCylinder : public CCycle
{
public:
    using CCycle::CCycle;
    CCylinder() :m_height(0) {}
    CCylinder(double radius, double height) {
        SetRadius(radius);
        SetHeight(height);
    }
    void SetHeight(double height)
    {
        m_height = height;
    }
    double GetHeight()
    {
        return m_height;
    }
    double GetArea()
    {
        return CCycle::GetArea() + m_height * 2 * PI * GetRadius();
    }
    double GetVolumn()
    {
        return CCycle::GetArea() * m_height;
    }
private:
    double m_height;
};

class CRectangle : public CGraph
{
public:
    using CGraph::CGraph;
    CRectangle() :m_length(0), m_width(0) {}
    CRectangle(double length, double width) {
        SetLength(length);
        SetWidth(width);
    }
    void SetLength(double length)
    {
        m_length = length;
    }
    void SetWidth(double width)
    {
        m_width = width;
    }
    double GetLength()
    {
        return m_length;
    }
    double GetWidth()
    {
        return m_width;
    }
    double GetArea()
    {
        return m_length * m_width;
    }
private:
    double m_length;
    double m_width;
};
int main()
{
    CCycle c(40);
    CRectangle r(10, 100);
    CCylinder cy(10, 100);
    cout << "cycle area: " << c.GetArea() << endl;
    cout << "rectangle area: " << r.GetArea() << endl;
    cout << "cylinder area: " << cy.GetArea() << endl;
    cout << "cylinder volumn: " << cy.GetVolumn() << endl;
    return 0;
}

