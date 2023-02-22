#pragma once
#include <string>
using namespace std;

class CException
{
public:
    CException(string str) :m_reason(str) {}
    string Reason() const
    {
        return m_reason;
    }
private:
    string m_reason;
};