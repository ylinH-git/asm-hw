// hashmap.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CHashmap.h"
using namespace std;

int main()
{
    CHashmap<string, int> map;
    map.Insert("hello",1);
    map.Insert("test", 2);
    map.Insert("aaaa", 3);
    map.Insert("bbbb", 4);
    map.Insert("cccc", 5);

    map["hello"] = 100;

    CHashmap<string, int> map2(map);
    CHashmap<string, int> map3(std::move(map));
    CHashmap<string, int> map4;
    map4 = map2;
    CHashmap<string, int> map5;
    map5 = std::move(map3);

    map4.Clear();
#if 0
    map.Delete("hello", 100);
    map.Delete("test", 2);
    map.Delete("aaaa", 3);
    map.Delete("bbbb", 4);
    map.Delete("cccc", 5);
    map.Delete("ssdf", 5);
#endif // 0

    return 0;
}
