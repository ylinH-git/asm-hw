// CAnimal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

class CAnimal
{
public:
    CAnimal() :m_dbHeight(NULL), m_dbWeight(NULL)
    {
        cout << "CAnimal()" << endl;
    }
    CAnimal(double dbHeight, double dbWeight):m_dbHeight(dbHeight), m_dbWeight(dbWeight) 
    {
        cout << "CAnimal(double dbHeight, double dbWeight)" << endl;
        if (dbHeight == NULL || dbWeight == NULL)
        {
            this->~CAnimal();
            throw(string("忘记设置关键信息"));
        }
    }
    void Show()
    {
        if (m_dbHeight == NULL || m_dbWeight == NULL)
        {
            throw(string("忘记设置关键信息"));
        }
        else 
        {
            cout << "Height: " << m_dbHeight << endl;
            cout << "Weight: " << m_dbWeight << endl;
        }
       
    }
    ~CAnimal()
    {
        cout << "~CAnimal" << endl;
    }
private:
    double m_dbHeight;
    double m_dbWeight;

};
int main()
{
    try
    {
        CAnimal c = CAnimal(123, 45);
        CAnimal c1 = CAnimal();
        c1.Show();
    }
    catch (const string e)
    {
        cout << e << endl;
    }
    std::cout << "Hello World!\n";
}

