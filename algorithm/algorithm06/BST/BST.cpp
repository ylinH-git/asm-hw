// BST.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CBinarySearchTree.h"


int main()
{
  CBinarySearchTree bst;
  bst.Insert(50);
  bst.Insert(25);
  bst.Insert(75);
  bst.Insert(13);
  bst.Insert(42);
  bst.Insert(60);
  bst.Insert(88);
  bst.TraverseMid();
  bst.TraverseFront();
  bst.TraverseBack();
  bst.TraverseLayer();
  std::cout << "Hello World!\n";
}


