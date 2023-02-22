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
    struct tm t;   //tm结构指针
    time_t now;  //声明time_t类型变量
    time(&now);      //获取系统日期和时间
    localtime_s(&t, &now);   //获取当地日期和时间
    int y = 1900 + t.tm_year;
    int m = t.tm_mon + 1;
    int d = t.tm_mday;
    int age = y - year;
    return m - month >= 0 && d - day >= 0 ? age : age - 1;
}           
