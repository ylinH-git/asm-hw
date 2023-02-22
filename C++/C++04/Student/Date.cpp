#include <iostream>
#include <cmath>
#include <ctime>
#include "Date.h"


void Date::SetDate(int y, int m, int d)
{
    year = y;
    month = m;
    day = d;
}

bool Date::GetLeapYear()
{
    return year % 4 ? false : true;
}

int Date::GetWeekDay()
{
    int y = year % 100;
    int c = year / 100;
    return ((int)(y + (int)floor(y / 4)
        + (int)floor(c / 4) - 2 * c
        + floor((26 * (month + 1) / 10))
        + day - 1) % 7 + 7) % 7;
}

int Date::GetAge()
{
    struct tm t;   //tm�ṹָ��
    time_t now;  //����time_t���ͱ���
    time(&now);      //��ȡϵͳ���ں�ʱ��
    localtime_s(&t, &now);   //��ȡ�������ں�ʱ��
    int y = 1900 + t.tm_year;
    int m = t.tm_mon + 1;
    int d = t.tm_mday;
    int age = y - year;
    return m - month >= 0 && d - day >= 0 ? age : age - 1;
}           
