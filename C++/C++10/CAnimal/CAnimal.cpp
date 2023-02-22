// CAnimal.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;
class CAnimal
{
public:
    CAnimal():m_h(0), m_w(0)
    {}
    CAnimal(double height, double weight)
    {
        SetHeight(height);
        SetWeight(weight);
    }
    void SetHeight(double height)
    {
        m_h = height;
    }
    void SetWeight(double weight)
    {
        m_h = weight;
    }
    double GetHeight() const
    {
        return m_h;
    }
    double GetWeight() const
    {
        return m_w;
    }
private:
    double m_h;
    double m_w;
};

class CDog: public CAnimal
{
public:
    using CAnimal::CAnimal;
    CDog() :m_color("#ffffff"){}
    CDog(double height, double weight, const string& color){
        SetHeight(height);
        SetWeight(weight);
        SetColor(color);
    }
    void SetColor(const string& color)
    {
        m_color = color;
    }
    string GetColor() const
    {
        return m_color;
    }
private:
    string m_color;
};
int main()
{
    CDog dog(100, 50, "#000000");
    CDog dog1;
    cout << "height: " << dog.GetHeight() << endl;
    cout << "weight: " << dog.GetWeight() << endl;
    cout << "color: " << dog.GetColor() << endl;
}

