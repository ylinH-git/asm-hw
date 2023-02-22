// CStack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

class CStack {
public:
    CStack(int n) :m_n(n) {};
    void* operator new(size_t nSize) = delete;
    void operator delete(void* stack) = delete;
private:
    int m_n;
};
int main()
{
    //CStack stack = new CStack(1)
    CStack stack(123);
    std::cout << "Hello World!\n";
}
