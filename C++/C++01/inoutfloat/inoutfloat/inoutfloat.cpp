// inoutfloat.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
int main()
{
    float fNum;
    cout << "请输入一个科学计数法浮点数\r\n";
    cin >> scientific >> fNum;
    cout << fNum << endl;
    cout << "定点法表示： " << fixed << fNum << endl;;

    cout << "请输入定点浮点数\r\n";
    cin >> fixed >> fNum;
    cout << fNum << endl;
    cout << "科学计数法表示： " << scientific << fNum << endl;;

}
