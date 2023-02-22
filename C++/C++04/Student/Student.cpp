// classpoint.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include "Date.h"
using namespace std;

class Student {
    const char* pName;
    Date DBirthday;
    int nAge;
public:
    void SetStudent(const char * name, Date *birthday)
    {
        pName = name;
        DBirthday = *birthday;
        nAge = DBirthday.GetAge();
    }
    int GetAge()
    {
        return nAge;
    }
    const char* GetName()
    {
        return pName;
    }
};
int main()
{   
    Student stu1;
    Date d1;
    d1.SetDate(1994, 3, 5);
    stu1.SetStudent("zhangsan", &d1);
    cout << "My name is " << stu1.GetName() << ", my age is " << stu1.GetAge() << endl;

    return 0;
}
