// classpoint.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cmath>
using namespace std;

#define PI 3.1415
class Circle {
    double dbRadius;
public:
    void SetCircle(double r)
    {
        dbRadius = r;
    }

    double GetArea()
    {
        return dbRadius * dbRadius * PI;
    }
};
int main()
{
    Circle c1;
    c1.SetCircle(3);
    cout << "Area of Circle(3): " << c1.GetArea() << endl;

    return 0;
}
