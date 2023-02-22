// classABC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
class A
{
public:
    A() {};
    virtual void Fun(int n ) {}
    virtual void Fun(double db) {}
};

class B :public  A
{
public:
    B() {};
    virtual void Fun1(int n) {}
    virtual void Fun(int n) {}
};

class C :public  B
{
public:
    C() {};
    virtual void Fun1(double db) {}
    virtual void Fun2(int n) {}
};
int main(){
    C c;
    c.Fun2(2);
    std::cout << "Hello World!\n";
}

