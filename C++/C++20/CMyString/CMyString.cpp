// CMyString.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CString.h"
#include "../CException/CException.h"
int main()
{
    CString str("hello");
	try
	{
		str[7];
	}
	catch (const CException& e)
	{
		cout << e.Reason() << endl;
	}
}
