// class.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

class CSingle {
public:
    static CSingle& getCSingle()
    {
        return  single;
    }
private:
    int m_n = 13;
    static CSingle single;
    CSingle() {};
    CSingle(const CSingle &single) {};
};
CSingle CSingle::single = CSingle();

class CHeap {
public:
    static CHeap* getCHeap()
    {
        CHeap* heap = new CHeap();
        return  heap;
    }
private:
    int m_n = 14;
    CHeap() {};
    CHeap(const CHeap& heap) {};
};

class CStack {
public:
    static CStack* getCStack()
    {
        CStack stack = CStack();
        return  &stack;
    }
private:
    int m_n = 12;
    CStack() {};
    CStack(const CStack& heap) {};
};

int main()
{
    CSingle& a = CSingle::getCSingle();
    CSingle& b = CSingle::getCSingle();

    CHeap* c = CHeap::getCHeap();
    CHeap* d = CHeap::getCHeap();

    CStack* e = CStack::getCStack();
    CStack* f = CStack::getCStack();
    std::cout << "Hello World!\n";
}

