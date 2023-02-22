// ascii.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
int main()
{
    for (unsigned int i = 0; i < 256; i++)
    {
        cout << dec << i << " ";
        cout << (char) i << " ";
        cout << hex << i << " ";
        cout << oct << i << " ";
        cout << endl;
    }
}
