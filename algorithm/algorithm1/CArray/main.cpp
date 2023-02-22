// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "CString.h"
#include "CArray.h"


int main()
{
  CArray<CString> ary;
  ary.Insert("test", 0);
  ary.Insert("test1", 1);
  ary.Insert("test2", 2);
  ary.Insert("test5123123", 2 );
  ary.Delete(1);
  ary.Delete(0);
  ary.Delete(2);
  ary.InsertHead("testHead1");
  ary.DeleteHead();
  ary.InsertTail("testTail2");
  ary.DeleteTail();
  int i = ary.Find("test5123123");
  int j = ary.Find("testTail2");
  CString s = ary[0];
  ary.Clear();
  CArray<int> ary2;
  ary2.Insert(1, 0);

}

