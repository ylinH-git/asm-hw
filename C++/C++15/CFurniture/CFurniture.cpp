// CFurniture.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
class CFurniture
{
public:
    virtual void Use() = 0;
};

class CSofa
{
public:
    void sit()
    {
        cout << "sit" << endl;
    }
};

class CBed
{
public:
    void lie()
    {
        cout << "lie" << endl;
    }
};

class CSofaBed: public CSofa, public CBed
{
public:
    void lie()
    {
        cout << "lie" << endl;
    }
};

class CSofaBedElse
{
public:
    CSofaBedElse() {}
    void sit()
    {
        m_sofa.sit();
    }
    void lie()
    {
        m_bed.lie();
    }
private:
    CSofa m_sofa;
    CBed m_bed;
};
int main()
{
    CSofaBed sofabed;
    sofabed.sit();
    sofabed.lie();
    CSofaBed sofabedelse;
    sofabedelse.sit();
    sofabedelse.lie();
    return 0;
}
