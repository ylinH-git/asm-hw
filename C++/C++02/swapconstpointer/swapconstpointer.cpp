// swapconstpointer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
void Swap(int *const x, int* const y)
{
    int nTemp = *x;
    *x = *y;
    *y = nTemp;
}
int main()
{
    int x = 9;
    int y = 10;

    cout << "(before)x: " << x << " " << "y: " << y << endl;
    Swap(&x, &y);
    cout << "(after swap)x: " << x << " " << "y: " << y << endl;
}