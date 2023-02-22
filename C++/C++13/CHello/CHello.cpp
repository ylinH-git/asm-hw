// CHello.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

class CHello
{
public:
    explicit CHello() {}
    void operator()()
    {
        std::cout << m_str;
    }
private:
    const char* m_str = "helloworld";
};

int main()
{
    CHello hello;
    hello();
    return 0;
}
