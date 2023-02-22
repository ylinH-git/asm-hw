// inline.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
inline bool isNum(char c)
{
    if ((int)c <= 57 && (int)c >= 48)
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool isAlpha(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool isEven(int x)
{
    if (!(x % 2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool isLeapYear(int year)
{
    if (!(year % 4))
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline char upper(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return (char)c - 32;
    }
    else 
    {
        return c;
    }
}

inline char lower(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return (char)c + 32;
    }
    else
    {
        return c;
    }
}
int main()
{
    cout << "1 is number: " << (isNum('1') ? "true" : "false") << endl;
    cout << "a is number: " << (isNum('a') ? "true" : "false") << endl;
    cout << "a is alphabet: " << (isAlpha('a') ? "true" : "false") << endl;
    cout << "1 is alphabet: " << (isAlpha('1') ? "true" : "false") << endl;

    cout << "30 is even: " << (isEven(30) ? "true" : "false") << endl;
    cout << "31 is even: " << (isEven(31) ? "true" : "false") << endl;
    cout << "2000 is leap year: " << (isLeapYear(2000) ? "true" : "false") << endl;
    cout << "2001 is leap year: " << (isLeapYear(2001) ? "true" : "false") << endl;

    cout << "a to " << upper('a') << endl;
    cout << "A to " << lower('A') << endl;
}
