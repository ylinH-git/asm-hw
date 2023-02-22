// noExtend.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
class CParent 
{
private:
    CParent() {}
    CParent(const CParent& parent) {}
};

class CChild:public CParent
{
public:
    using CParent::CParent;
    CChild() {}
};
int main()
{
    CChild child;
}
