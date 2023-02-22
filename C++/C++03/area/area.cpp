// area.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
#define PI 3.1415
double GetCircleArea(double r = 10)
{
    return r * r * PI;
}

double GetRectArea(double x = 10, double y = 5)
{
    return x * y;
}

double GetCuboidArea(double h, double x = 10, double y = 5)
{
    return  (x * y + x * h + y * h) * 2;
}

int main()
{
    cout << "圆面积：10 * 10 * 3.1415 = " << GetCircleArea() << endl;
    cout << "长方形面积：10 * 5 = " << GetRectArea() << endl;
    cout << "(h = 10)长方体面积：(10 * 5 + 10 * h + 5 * h) * 2 = " << GetCuboidArea(10) << endl;
}

