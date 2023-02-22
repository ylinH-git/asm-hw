// classpoint.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cmath>
using namespace std;

class Rect {
    double dbLength;
    double dbWidth;
public:
    Rect(double x, double y)
    {
        dbLength = x;
        dbWidth = y;
    }

    void SetRect(double x, double y)
    {
        dbLength = x;
        dbWidth = y;
    }

    double GetArea()
    {
        return dbLength * dbWidth;
    }

    ~Rect()
    {
        dbLength = 0;
        dbWidth = 0;
    }
};
int main()
{
    Rect r1(1, 2);
    cout << "Area of Rect(1, 2): " << r1.GetArea() << endl;

    return 0;
}
