// CPerson.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CPerson.h"

using namespace std;

CTeacher::CTeacher() :m_name(""), m_age(0), m_gender(eGender::none), m_address(""), m_tel(""), m_job_title("") {}
CTeacher::CTeacher(int age, const string& name, eGender gender, const string& job_title, const string& address, const string& tel)
    :m_name(name), m_age(age), m_gender(gender), m_address(address), m_tel(tel), m_job_title(job_title) {}
void CTeacher::SetAge(int age)
{
    m_age = age;
}
void CTeacher::SetName(const string& name)
{
    m_name = name;
}
void CTeacher::SetGender(eGender gender)
{
    CTeacher::m_gender = gender;
}
void CTeacher::SetTel(const string& tel)
{
    m_name = tel;
}
void CTeacher::SetAddr(const string& addr)
{
    m_name = addr;
}
void CTeacher::SetJobTitle(const string& job_title)
{
    m_job_title = job_title;
}
int CTeacher::GetAge()
{
    return m_age;
}
string CTeacher::GetName()
{
    return m_name;
}
string CTeacher::GetGender()
{
    switch (m_gender)
    {
    case eGender::male:
        return "male";
    case eGender::female:
        return "female";
    default:
        return "none";
    }
}
string CTeacher::GetTel()
{
    return m_tel;
}
string CTeacher::GetAddr()
{
    return m_address;
}
string CTeacher::GetJobTitle()
{
    return m_job_title;
}

void CTeacher::Display()
{
    cout << "name: " << GetName() << endl;
    cout << "age: " << GetAge() << endl;
    cout << "gender: " << GetGender() << endl;
    cout << "job title: " << GetJobTitle() << endl;
    cout << "address: " << GetAddr() << endl;
    cout << "tel: " << GetTel() << endl;
}

CCadre::CCadre() :m_name(""), m_age(0), m_gender(eGender::none), m_address(""), m_tel(""), m_job("") {}
CCadre::CCadre(int age, const string& name, eGender gender, const string& job, const string& address, const string& tel)
    : m_name(name), m_age(age), m_gender(gender), m_address(address), m_tel(tel), m_job(job) {}
void CCadre::SetAge(int age)
{
    m_age = age;
}
void CCadre::SetName(const string& name)
{
    m_name = name;
}
void CCadre::SetGender(eGender gender)
{
    CCadre::m_gender = gender;
}
void CCadre::SetTel(const string& tel)
{
    m_name = tel;
}
void CCadre::SetAddr(const string& addr)
{
    m_name = addr;
}
void CCadre::SetJob(const string& job)
{
    m_job = job;
}
int CCadre::GetAge()
{
    return m_age;
}
string CCadre::GetName()
{
    return m_name;
}
string CCadre::GetGender()
{
    switch (m_gender)
    {
    case eGender::male:
        return "male";
    case eGender::female:
        return "female";
    default:
        return "none";
    }
}
string CCadre::GetTel()
{
    return m_tel;
}
string CCadre::GetAddr()
{
    return m_address;
}
string CCadre::GetJob()
{
    return m_job;
}

CTeacherCadre::CTeacherCadre():m_salary(0){}

CTeacherCadre::CTeacherCadre(
    int age, 
    const string& name, 
    eGender gender, 
    const string& job, 
    const string& job_title,
    const string& address, 
    const string& tel, 
    double salary
):CTeacher(age, name, gender, job_title, address, tel), m_salary(salary)
{
    SetJob(job);
}

void CTeacherCadre::SetSalary(double salary)
{
    m_salary = salary;
}

double CTeacherCadre::GetSalary()
{
    return m_salary;
}

void CTeacherCadre::Show()
{
    Display();
    cout << "job: " << GetJob() << endl;
    cout << "salary: " << GetSalary() << endl;
}
