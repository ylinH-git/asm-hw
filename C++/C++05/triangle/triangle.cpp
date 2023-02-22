// classpoint.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cmath>
using namespace std;

class Triangle {
    double dbX;
    double dbY;
    double dbZ;
public:
    Triangle(double x, double y, double z)
    {
        dbX = x;
        dbY = y;
        dbZ = z;
    }
    void SetTriangle(double x, double y, double z)
    {
        dbX = x;
        dbY = y;
        dbZ = z;
    }

    double GetArea()
    {
        double p = (dbX + dbY + dbZ) / 2;
        return sqrt(p * (p - dbX) * (p - dbY) * (p - dbZ));
    }

    ~Triangle()
    {
        dbX = 0;
        dbY = 0;
        dbZ = 0;
    }
};
int main()
{
    Triangle t1(4, 5, 6);
    cout << "Area ofTriangle(4, 5, 6): " << t1.GetArea() << endl;

    return 0;
}
