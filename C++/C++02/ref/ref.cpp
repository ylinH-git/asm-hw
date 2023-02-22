// ref.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
void PrintfIntRef(int& x)
{
    cout << x << endl;
}
void PrintfCharPointfRef(char * &pTextR)
{
    cout << pTextR << endl;
}
int main()
{
    int nInt = 100;
    int& nIntR = nInt;
    PrintfIntRef(nIntR);
    char cText[20] = "Hello World";
    char* pText = cText;
    char*& pTextR = pText;
    PrintfCharPointfRef(pTextR);
}
