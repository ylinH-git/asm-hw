// StringClass.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include "CMyString.h"


using namespace std;
int main(int argc, char** argv)
{
    CMyString str;
    CMyString str0("HELLO WORLD");

    CMyString str1(str);
    CMyString str2(str0);


    str.Append("hello");
    str.Append(nullptr);
    str0.Append("99999999999");
    str0.Append("745");

    str.Append("hello")
        .Append(" ")
        .Append(12)
        .Append(" ")
        .Append(" world");

    int nF1 = str.Find("12");
    int nF2 = str.ReverseFind("12");

    CMyString subStr1 = str.Left(4);
    CMyString subStr2 = str.Right(4);
    CMyString subStr3 = str.Mid(4, 4);

    CMyString subStr4 = str.Delete(4, 4);
    CMyString formatStr = str.Format("%d, %f", 1, 1.0f);
#if 0

    auto nLen = str.Len();
    nLen = str2.Len();

    //str.At(1);
    //str2.At(89);
    auto ch = str2.At(2);
    str2.At(2) = 'C';

    str.Copy("hello");
    str2.Copy("ttttt");
    str1.Copy(nullptr);
    str2.Copy(nullptr);
#endif // 0




    std::cout << "Hello World!\n";
}
