#pragma once
#include <string>
using namespace std;
enum class eGender {
    male,
    female,
    none
};

enum class eStuDegree {
    undergraduate,
    master,
    doctor
};
class CPerson
{
public:
    virtual void SetAge(int age) = 0;
    virtual void SetName(const string& name) = 0;
    virtual void SetGender(eGender gender) = 0;
    virtual int GetAge() = 0;
    virtual string GetName() = 0;
    virtual string GetGender() = 0;
};


class CTeacher : public CPerson
{
public:
    CTeacher();
    CTeacher(int age, const string& name, eGender gender, const string& job_title, const string& address, const string& tel);
    void SetAge(int age);
    void SetName(const string& name);
    void SetGender(eGender gender);
    void SetTel(const string& tel);
    void SetAddr(const string& addr);
    void SetJobTitle(const string& job_title);
    int GetAge();
    string GetName();
    string GetGender();
    string GetTel();
    string GetAddr();
    string GetJobTitle();
    void Display();
private:
    int m_age;
    string m_name;
    eGender m_gender;
    string m_job_title;
    string m_address;
    string m_tel;
};

class CCadre : public CPerson
{
public:
    CCadre();
    CCadre(int age, const string& name, eGender gender, const string& job_title, const string& address, const string& tel);
    void SetAge(int age);
    void SetName(const string& name);
    void SetGender(eGender gender);
    void SetTel(const string& tel);
    void SetAddr(const string& addr);
    void SetJob(const string& job);
    int GetAge();
    string GetName();
    string GetGender();
    string GetTel();
    string GetAddr();
    string GetJob();
private:
    int m_age;
    string m_name;
    eGender m_gender;
    string m_job;
    string m_address;
    string m_tel;
};

class CTeacherCadre : public CTeacher, public CCadre
{
public:
    CTeacherCadre();
    CTeacherCadre(int age, 
        const string& name, 
        eGender gender, 
        const string& job,
        const string& job_title,
        const string& address, 
        const string& tel,
        double salary);
    void SetSalary(double salary);
    double GetSalary();
    void Show();
private:
    double m_salary;
};