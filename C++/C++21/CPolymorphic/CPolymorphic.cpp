// CPolymorphic.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <functional>
using namespace std;
class CRole
{
public:
    CRole() {
        Move = []() {
            cout << "移动" << endl;
        };
        Attack = []() {
            cout << "攻击" << endl;
        };
    }
    function<void()> Move;
    function<void()> Attack;
};

class CWarrior: public CRole
{
public:
    CWarrior() {
        Move = []() {
            cout << "跑" << endl;
        };
        Attack = []() {
            cout << "近战" << endl;
        };
    }
};

class CMagic : public CRole
{
public:
    CMagic() {
        Move = []() {
            cout << "飞" << endl;
        };
        Attack = []() {
            cout << "火球术" << endl;
        };
    }
};

class CMonster : public CRole
{
public:
    CMonster() {
        Move = []() {
            cout << "爬" << endl;
        };
        Attack = []() {
            cout << "撕咬" << endl;
        };
    }
};

class CTank : public CRole
{
public:
    CTank()
    {
        Move = []() {
            cout << "行驶" << endl;
        };
        Attack = []() {
            cout << "大炮" << endl;
        };
    }
};
int main()
{
    CRole* r1 = new CTank();
    r1->Move();
    delete r1;
    std::cout << "Hello World!\n";
}
