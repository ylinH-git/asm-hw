// CException.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CException.h"
using namespace std;

void Func1()
{
    throw CException("error");
}
int main()
{
    try
    {
        Func1();
    }
    catch (const CException& e)
    {
        cout << e.Reason() << endl;
    }
    return 0;
}