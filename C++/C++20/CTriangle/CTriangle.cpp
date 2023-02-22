// CTriangle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
class CTriangle
{
public:
    CTriangle(double dbA, double dbB, double dbC) :m_a(dbA), m_b(dbB), m_c(dbC)
    {
        cout << "CTriangle(double dbA, double dbB, double dbC)" << endl;
        if (dbA == NULL || dbB == NULL || dbC == NULL)
        {
             this->~CTriangle();
            throw(string("边长为0"));
        }
        bool isTriangle = true;
        double dbArr[3] = { dbA, dbB, dbC };
        double dbMax = 0;
        double dbSum = 0;
        for (int i = 0; i < 3; i++)
        {
            if (dbArr[i] > dbMax)
            {
                dbMax = dbArr[i];
            }
            dbSum += dbArr[i];
        }
        if ((dbSum - dbMax) <= dbMax)
        {
            isTriangle = false;
        }
        if (!isTriangle)
        {
            this->~CTriangle();
            throw(string("3边长构不成三角形"));
        }
    }
    ~CTriangle()
    {
        cout << "~CTriangle" << endl;
    }
private:
    double m_a;
    double m_b;
    double m_c;
};
int main()
{
    try
    {
        //CTriangle(123, 345, 789);
        CTriangle(123, 345, 0);
    }
    catch (const string e)
    {
        cout << e << endl;
    }
}
