#pragma once
class Date
{
    int year;
    int month;
    int day;
public:
    Date(int y, int m, int d);
    void SetDate(int y, int m, int d);
    bool GetLeapYear();
    int GetWeekDay();
    int GetAge();
    ~Date();
};

