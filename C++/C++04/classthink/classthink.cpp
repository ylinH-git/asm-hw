// classpoint.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cmath>
using namespace std;
#define GET_OFFSET(s, m) (size_t)&((s *)NULL)->m
#define GET_MEMBER(c, type, size) *((type *)((unsigned char *)&c + size))
class Rect {
    double dbLength;
    double dbWidth;
public:
    void SetRect(double x, double y)
    {
        dbLength = x;
        dbWidth = y;
    }

    double GetArea()
    {
        return dbLength * dbWidth;
    }

    void _stdcall ChangeThis()
    {   
        int x = 1;
        int* pThis = (int*)&x + 5;
        *pThis = 0x00000000;
        cout << this << endl;
    }
};

struct SRect {
    double dbLength;
    double dbWidth;
};
int main()
{
    Rect r1;
    r1.SetRect(1, 2);
    // 通过指针访问dbLength
    cout << "Rect's dbLength is " << GET_MEMBER(r1, double, GET_OFFSET(SRect, dbLength)) << endl;
    cout << "Rect's dbWidth is " << GET_MEMBER(r1, double, GET_OFFSET(SRect, dbWidth)) << endl;

    Rect* rp = &r1;
    // 在成员函数内修改this指针
    r1.ChangeThis();
    return 0;
}
