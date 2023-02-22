// CString.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CString.h"
using namespace std;
int main()
{
    CString str1 = CString::ValueOf(true);
    CString str2(str1);
    CString str3(str1);
    CString str4(str1);
    CString str5(str1);
    CString str6(str1);

    str1.SetAt('a', 0);
    str2.Copy("1234");
    str3.Append('b').Append('c');
    str5.Delete(1, 2);

    CString str7;
    str7.Copy("456");

    return 0;
}
