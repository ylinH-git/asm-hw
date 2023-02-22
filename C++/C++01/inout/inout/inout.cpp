// inout.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

int main()
{
    // 题目2-4
    int nOct, nDec, nHex;

    // 输入输出八进制整数
    cout << "请输入一个8进制数\r\n";
    cin >> oct >> nOct;
    cout << dec << nOct << endl;
    cout << "请输入一个10进制数\r\n";
    cin >> dec >> nDec;
    cout << dec << nDec << endl;
    cout << "请输入一个16进制数\r\n";
    cin >> hex >> nHex;
    cout << dec << nHex << endl;
}
