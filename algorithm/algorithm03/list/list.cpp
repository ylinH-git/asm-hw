// linklist.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

#include "CList.h"

int main()
{
  CList lst;
  lst.Insert(7, 0);
  lst.Insert(8, 0);
  lst.Insert(9, 0);
  lst.Insert(0x10, 0);
  lst.Insert(0x11, 4);
  lst.Insert(0x12, 3);

  for (size_t i = 0; i < lst.Size(); i++)
  {
    cout << lst[i] <<endl;
  }

  if (lst.Find(9) == CList::INVALID_IDX)
  {
    //失败
  }

  // 0x10 9 8 0x12 7 0x11
#if 0
  lst.Delete(0);// 9 8 0x12 7 0x11
  lst.Delete(0);// 8 0x12 7 0x11
  lst.Delete(2);// 8 0x12  0x11
  lst.Delete(2);// 8 0x12 
  lst.Delete(1);// 8
  lst.Delete(0);
#endif // 0
  CList lst2 = lst;
  CList lst3;
  lst3 = lst;
  CList lst4(std::move(lst2));
  lst.Clear();

  //lst.Delete(0);
  std::cout << "Hello World!\n";
}


