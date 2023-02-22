// CPerson.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
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
    CPerson() :m_age(0), m_name(""), m_gender(eGender::none) {}
    CPerson(int age, const string& name, eGender gender) {
        SetAge(age);
        SetName(name);
        SetGender(gender);
    }
    void SetAge(int age)
    {
        m_age = age;
    }
    void SetName(const string& name)
    {
        m_name = name;
    }
    void SetGender(eGender gender)
    {
        m_gender = gender;
    }
    int GetAge()
    {
        return m_age;
    }
    string GetName()
    {
        return m_name;
    }
    string GetGender()
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
private:
    int m_age;
    string m_name;
    eGender m_gender;
};


class CTeacher: public CPerson
{
public:
    using CPerson::CPerson;
    CTeacher() :m_job_title(""), m_course("") {}
    CTeacher(int age, const string& name, eGender gender, const string &job_title, const string &cource) :m_job_title(""), m_course("") {
        SetAge(age);
        SetName(name);
        SetGender(gender);
        SetJobTitle(job_title);
        SetCourse(cource);
    }
    void SetJobTitle(const string &job_title)
    {
        m_job_title = job_title;
    }
    void SetCourse(const string &cource)
    {
        m_course = cource;
    }
    string GetJobTitle()
    {
        return m_job_title;
    }
    string GetCourse()
    {
        return m_course;
    }
private:
    string m_job_title;
    string m_course;
};

class CStudent : public CPerson
{
public:
    using CPerson::CPerson;
    CStudent() :m_major(""), m_class(""), m_degree(eStuDegree::undergraduate) {}
    CStudent(int age, const string& name, eGender gender, const string& major, const string& classname, eStuDegree degree) {
        SetAge(age);
        SetName(name);
        SetGender(gender);
        SetMajor(major);
        SetClass(classname);
        SetDegree(degree);
    }
    void SetMajor(const string& major)
    {
        m_major = major;
    }
    void SetClass(const string& classname)
    {
        m_class = classname;
    }
    void SetDegree(eStuDegree degree)
    {
        m_degree = degree;
    }
    string GetMajor()
    {
        return m_major;
    }
    string GetClass()
    {
        return m_class;
    }
    string GetDegree()
    {
        switch (m_degree)
        {
        case eStuDegree::master:
            return "master";
        case eStuDegree::doctor:
            return "doctor";
        default:
            return "undergraduate";
        }
    }
private:
    string m_major;
    string m_class;
    eStuDegree m_degree;
};
int main()
{
    CTeacher teacher(27, "zhangsan", eGender::male, "高级教师", "math");
    cout << "Teacher:" << endl;
    cout << "name: " << teacher.GetName() << endl;
    cout << "age: " << teacher.GetAge() << endl;
    cout << "gender: " << teacher.GetGender() << endl;
    cout << "job title: " << teacher.GetJobTitle() << endl;
    cout << "course: " << teacher.GetCourse() << endl;


    CStudent student(20, "zhangsan", eGender::male, "math", "14届15班", eStuDegree::master);
    cout << "Student:" << endl;
    cout << "name: " << student.GetName() << endl;
    cout << "age: " << student.GetAge() << endl;
    cout << "gender: " << student.GetGender() << endl;
    cout << "major: " << student.GetMajor() << endl;
    cout << "class: " << student.GetClass() << endl;
    cout << "degree: " << student.GetDegree() << endl;
}

