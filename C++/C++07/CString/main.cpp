// CString.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CString.h"
using namespace std;
int main()
{
    CString str1 = CString::ValueOf(true);
    CString str2 = CString::ValueOf('a');
    CString str3 = CString::ValueOf(3.5);
    CString str4 = CString::ValueOf(4.5f);
    CString str5 = CString::ValueOf(123);
    CString str6 = CString::ValueOf((long)123);
    CString str7 = CString::ValueOf((short)123);
    CString str8 = CString::ValueOf("123");
    CString str9 = CString::CopyValueOf("123456789", 1, 4);
    CString str10 = CString::StringFromFormat("%d", 123);
    cout << str1.ToString() << endl;
    cout << str2.ToString() << endl;
    cout << str3.ToString() << endl;
    cout << str4.ToString() << endl;
    cout << str5.ToString() << endl;
    cout << str6.ToString() << endl;
    cout << str7.ToString() << endl;
    cout << str8.ToString() << endl;
    cout << str9.ToString() << endl;
    cout << str10.ToString() << endl;
}
