#pragma once
class Date
{
    int year;
    int month;
    int day;
public:
    void SetDate(int y, int m, int d);
    bool GetLeapYear();
    int GetWeekDay();
    int GetAge();
};

