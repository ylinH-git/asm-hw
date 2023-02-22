// classpoint.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cmath>
using namespace std;

class Point {
    double position[2];
public:
    Point(double x, double y) 
    {
        position[0] = x;
        position[1] = y;
    }
    void SetPosition(double x, double y)
    {
        position[0] = x;
        position[1] = y;
    }
    double* GetPosition()
    {
        return position;
    }
    double GetDistance(Point* p)
    {
        return sqrt(pow((p->position[0] - position[0]), 2)
            + pow((p->position[1] - position[1]), 2));
    }
    ~Point()
    {
        position[0] = 0;
        position[1] = 0;
    }
};
int main()
{
    Point p1{1 ,2};
    Point p2{1, 4};
    cout << "Distance between p1(1,2) and p2(1,4): " << p1.GetDistance(&p2) << endl;

    return 0;
}