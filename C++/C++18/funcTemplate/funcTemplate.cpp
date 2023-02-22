// funcTemplate.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
template<typename T>
// 比较大小
bool Bigger(T val1, T val2)
{
    return val1 > val2;
}


// 求闰年
template<typename T>
bool LeapYear(T year)
{
    bool isLeap = false;
    if (year % 100)
    {
        isLeap = !((int)year % 4);
    }
    else
    {
        isLeap = !((int)year % 400);
    }
    return isLeap;
}

// 求闰年（字符串）
template<>
bool LeapYear(const char* year)
{
    
    return LeapYear(atoi(year));
}

// 平方
template<typename T>
double Square(T val)
{
    return pow(val, 2);
}

// 三角形面积
template<typename T>
double TriangleArea(T a, T b, T c)
{
    T p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

// 绝对值
template<typename T>
T Abs(T a)
{
    return abs(a);
}

template<typename T>
class Compare
{
public:
    Compare(T val) : m_val(val) {};
    bool operator>(Compare<T>& val)
    {
        return m_val > val.m_val;
    }
    bool operator<(Compare<T>& val)
    {
        return m_val < val.m_val;
    }
    bool operator==(Compare<T>& val)
    {
        return m_val == val.m_val;
    }
    bool operator>=(Compare<T>& val)
    {
        return m_val >= val.m_val;
    }
    bool operator<=(Compare<T>& val)
    {
        return m_val <= val.m_val;
    }
    Compare<T> operator+(Compare<T>& val)
    {
        return m_val + val.m_val;
    }
    Compare<T> operator-(Compare<T>& val)
    {
        return m_val - val.m_val;
    }
private:
    T m_val;
};
int main()
{
    cout << boolalpha << Bigger(4.6, 7.4) << endl;
    cout << boolalpha << LeapYear(2000) << endl;
    cout << boolalpha << LeapYear("1900") << endl;
    cout  << Square(4) << endl;
    cout << TriangleArea<double>(4, 5, 6) << endl;
    cout << Abs(-4) << endl;
    Compare<string> a = string("a");
    Compare<string> b = string("b");
    Compare<int> nX = 10;
    Compare<int> nY = 20;
    cout << boolalpha << (a < b) << endl;
    cout << boolalpha << (nX > nY) << endl;
}