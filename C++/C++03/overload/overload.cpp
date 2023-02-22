// overload.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cmath>

using namespace std;
int GetMax(int x, int y)
{
    return x > y ? x : y;
}

float GetMax(float x, float y)
{
    return x > y ? x : y;
}

double GetMax(double x, double y)
{
    return x > y ? x : y;
}

int Squ(int x)
{
    return x * x;
}

float Squ(float x)
{
    return x * x;
}

double Squ(double x)
{
    return x * x;
}

int Abs(int x)
{
    return x > 0 ? x : -x;
}

float Abs(float x)
{
    return x > 0 ? x : -x;
}

double Abs(double x)
{
    return x > 0 ? x : -x;
}

double GetTriangleArea(int a, int b, int c)
{  
    double p = ((double)a + (double)b + (double)c) / 2;
    return sqrt(p * (p - (double)a) * (p - (double)b) * (p - (double)c));
}

double GetTriangleArea(float a, float b, float c)
{
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

double GetTriangleArea(double a, double b, double c)
{
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}
int main()
{
    cout << "Max(1, 2) = " << GetMax(1, 2) << endl;
    cout << "Max(1.123f, 2.24f) = " << GetMax(1.123f, 2.24f) << endl;
    cout << "Max(1.2340, 2.2342) = " << GetMax(1.2340, 2.2342) << endl;

    cout << "Squ(2) = " << Squ(2) << endl;
    cout << "Squ(2.24f) = " << Squ(2.24f) << endl;
    cout << "Squ(2.2342) = " << Squ(2.2342) << endl;

    cout << "Abs(-2) = " << Abs(-2) << endl;
    cout << "Abs(2.24f) = " << Abs(2.24f) << endl;
    cout << "Abs(-2.2342) = " << Abs(-2.2342) << endl;

    cout << "GetTriangleArea(4, 6, 4) = " << GetTriangleArea(4, 6, 4) << endl;
    cout << "GetTriangleArea(4.123f, 6.24f, 4.23f) = " << GetTriangleArea(4.123f, 6.24f, 4.23f) << endl;
    cout << "GetTriangleArea(4.2340, 4.2342, 4.234) = " << GetTriangleArea(4.2340, 4.2342, 4.234) << endl;

   
   // ? GetTriangleArea@@YANNNN@Z => double GetTriangleArea(double a, double b, double c)
   // ? GetTriangleArea@@YANHHH@Z => double GetTriangleArea(int a, int b, int c)
   // ? GetTriangleArea@@YANMMM@Z => double GetTriangleArea(float a, float b, float c)
   // N 表示返回 double类型 MMM 表示 参数位三个float

}
