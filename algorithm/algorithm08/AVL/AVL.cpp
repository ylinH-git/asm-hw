// BST.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CAVL.h"
using namespace std;


int main()
{
	CAVL<int> bst;

	for (auto val : { 68, 52, 73, 75, 81, 97, 59, 89, 90, 67, 27, 40, 88, 21, 49, 22, 30, 42 })
	{
		bst.Insert(val);
	}
	bst.Delete(68);
	bst.Delete(59);
	bst.Delete(49);
	bst.Delete(67);
	bst.TraverseLayer();

	CAVL<string> bst2;
	for (auto val : { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r" })
	{
		bst2.Insert(val);
	}
	bst2.TraverseLayer();
	bst2.Clear();
	std::cout << "Hello World!\n";
}


