// CMyString.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CString.h"
int main()
{
    CString str("hello");
    CString str1 = str + "test";
    CString str2 = "my" + str1;
    std::cout << str1.ToString() << std::endl;
    std::cout << str2.ToString() << std::endl;

    str += "sz";
    std::cout << str.ToString() << std::endl;

    str1 = "sz";
    std::cout << str1.ToString() << std::endl;
    str1 = str2;
    std::cout << str1.ToString() << std::endl;

    str[0] = 'a';
    std::cout << str.ToString() << std::endl;
     
    std::cout << (str == str1) << std::endl;
    std::cout << (str == str) << std::endl;

    std::cout << (str > str1) << std::endl;
    std::cout << (str < str1) << std::endl;

    std::cout << (const char*)str2 << std::endl;
}
