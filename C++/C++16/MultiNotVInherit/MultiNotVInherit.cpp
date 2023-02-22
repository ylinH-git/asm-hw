#include <iostream>
using namespace std;
class A {
public:
    A() {
        cout << "A()" << endl;
    };
    ~A()
    {
        cout << "~A()" << endl;
    };
    int m_nA = 0xaaaaaaaa;
};

class B {
public:
    B() {
        cout << "B()" << endl;
    };
    ~B()
    {
        cout << "~B()" << endl;
    };
    int m_nA = 0xbbbbbbbb;
};
class C {
public:
    C() {
        cout << "C()" << endl;
    };
    ~C()
    {
        cout << "~C()" << endl;
    };
    virtual void TestC1() {};
    virtual void TestC2() {};
    int m_nB = 0x0c0c0c0c;
};

class D {
public:
    D() {
        cout << "D()" << endl;
    };
    ~D()
    {
        cout << "~D()" << endl;
    };
    virtual void TestD1() {};
    virtual void TestD2() {};
    int m_nB = 0xdddddddd;
};


class E :public A, public B {
public:
    E() {
        cout << "E()" << endl;
    };
    ~E()
    {
        cout << "~E()" << endl;
    };
    int m_nB = 0xeeeeeeee;
};

class F :public C, public D {
public:
    F() {
        cout << "F()" << endl;
    };
    ~F()
    {
        cout << "~F()" << endl;
    };
    virtual void TestC1() {};
    virtual void TestD1() {};
    virtual void TestF1() {};
    int m_nB = 0xffffffff;
};
int main()
{
    E e;
    F f;
    return 0;
}
