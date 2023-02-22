#include <iostream>
using namespace std;

class Time {
    int hour;
    int minute;
    int second;
public:
    Time(int y, int m, int d)
    {
        hour = y;
        minute = m;
        second = d;
    }

    void SetDate(int y, int m, int d)
    {
        hour = y;
        minute = m;
        second = d;
    }

    int GetSecond()
    {
        return hour * 3600 + minute * 60 + second;
    }

    ~Time() {
        hour = 0;
        minute = 0;
        second = 0;
    }
};
int main()
{
    Time t1{13, 20, 59};
    cout << "Time {13, 20, 59} total second is: " << t1.GetSecond() << endl;
    return 0;
}
