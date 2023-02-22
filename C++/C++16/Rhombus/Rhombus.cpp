#include <iostream>
using namespace std;
class AB {
public:
    AB() {
        cout << "AB()" << endl;
    };
    ~AB()
    {
        cout << "~AB()" << endl;
    };
    int m_nAB = 0xabababab;
};
class A :virtual public AB{
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

class B :virtual public AB {
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
class CD {
public:
    virtual ~CD() = 0{};
    virtual void TestCD1()=0;
    virtual void TestCD2()=0;
    int m_nB = 0xcdcdcdcd;
};
class C :virtual public CD{
public:
    C() {
        cout << "C()" << endl;
    };
    ~C()
    {
        cout << "~C()" << endl;
    };
    virtual void TestCD2() {};
    virtual void TestC1() {};
    virtual void TestC2() {};
    int m_nB = 0x0c0c0c0c;
};

class D :virtual public CD {
public:
    D() {
        cout << "D()" << endl;
    };
    ~D()
    {
        cout << "~D()" << endl;
    };
    virtual void TestCD1() {};
    //virtual void TestD1() {};
    //virtual void TestD2() {};
    int m_nB = 0xdddddddd;
};


class E :public B, public A {
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
    //virtual void TestD1() {};
    virtual void TestF1() {};
    int m_nB = 0xffffffff;
};
int main()
{
    E e;
    F f;
    return 0;
}
