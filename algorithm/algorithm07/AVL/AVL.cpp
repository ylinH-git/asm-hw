// BST.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CAVL.h"


int main()
{
  CAVL bst;
#if 0
  // 测试左旋
  bst.Insert(1);
  bst.Insert(2);
  bst.Insert(3);
  bst.Insert(4);
  bst.Insert(5);
  bst.Insert(6);
  bst.Insert(7);
#endif // 0

#if 0
  // 测试右旋
  bst.Insert(7);
  bst.Insert(6);
  bst.Insert(5);
  bst.Insert(4);
  bst.Insert(3);
  bst.Insert(2);
  bst.Insert(1);
#endif // 0

#if 0
  // 测试
  // 1
  //  \
  //  3
  //  /
  // 2
  bst.Insert(1);
  bst.Insert(3);
  bst.Insert(2);
  bst.Insert(5);
  bst.Insert(4);
  bst.Insert(7);
  bst.Insert(6);
#endif // 0

#if 1
  // 测试
  //   7
  //  /
  // 5
  //  \
  //   6
  bst.Insert(7);
  bst.Insert(5);
  bst.Insert(6);
  bst.Insert(3);
  bst.Insert(4);
  bst.Insert(1);
  bst.Insert(2);
#endif // 0
  bst.TraverseLayer();
  std::cout << "Hello World!\n";
}


