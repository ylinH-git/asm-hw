// cout245.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
int main()
{

    // 题目5
    float fNum = 2.45;
    double dbNum = 2.45;
    cout << "2.45整型：" << dec << (int)2.45 << endl;
    cout << "2.45单精度浮点型：" << fNum << endl;
    cout << "2.45双精度浮点型：" << dbNum << endl;
    cout << "2.45科学计数法：" << scientific << fNum << endl;
}

