// CMyString.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CString.h"
using namespace std;
CString Foo()
{
	CString c = CString("Hello");
	return  c;
}
int main()
{
	CString str = Foo();
	return 0;
}

