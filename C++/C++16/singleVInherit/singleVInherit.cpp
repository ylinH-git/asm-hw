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
    virtual void Test() {};
    int m_nB = 0xbbbbbbbb;
};

class D : virtual public A {
public:
    D() {
        cout << "D()" << endl;
    };
    ~D()
    {
        cout << "~D()" << endl;
    };
    int m_nD = 0xdddddddd;
};


class E :virtual public B {
public:
    E() {
        cout << "E()" << endl;
    };
    ~E()
    {
        cout << "~E()" << endl;
    };
    void Test() {};
    virtual void Test2() {};
    int m_nB = 0xeeeeeeee;
};
int main()
{
    D d;
    E e;
    return 0;
}
