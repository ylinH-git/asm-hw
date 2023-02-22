// CTransportation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

class CTransportation
{
public:
    virtual void run() = 0;
    virtual ~CTransportation() = 0 {};
};

class CCar: public CTransportation
{
public:
    CCar()
    {
        cout << "CCar()" << endl;
    }
    ~CCar()
    {
        cout << "~CCar()" << endl;
    }
    void run()
    {
        cout << "地上跑" << endl;
    };
    int m_test = 0xffffffff; 
};

class CShip: public CTransportation
{
public:
    CShip()
    {
        cout << "CShip()" << endl;
    }
    ~CShip()
    {
        cout << "~CShip()" << endl;
    }
    void run()
    {
        cout << "水里跑" << endl;
    };
    int m_test = 0xeeeeeeee;
    
};

class CCarShip : public CShip, public CCar
{
public:
    CCarShip()
    {
        cout << "CCarShip()" << endl;
    }
    ~CCarShip()
    {
        cout << "~CCarShip()" << endl;
    }
    void run()
    {
        CCar::run();
        CShip::run();
    }
    int m_test = 0xbbbbbbbb;
};

int main()
{
    CCarShip* carship = new CCarShip();
    CCar* car = carship;
    CShip* ship = carship;
    CCarShip* car1= (CCarShip *) car;
    CCarShip* ship1 = (CCarShip*)ship;
    return 0;
}

