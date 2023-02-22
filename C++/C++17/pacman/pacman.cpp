// pacman.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <conio.h>
#include "CGame.h"
#include "common_def.h"
using namespace std;
int main()
{
    CGame game;
    int speed = 10;
    clock_t clockBeginTime = clock();
    clock_t clockEndTime = 0;
    while (true) {
        clockEndTime = clock();
        if (clockEndTime - clockBeginTime >= speed)
        {
            char ch = NULL;
            if (_kbhit()) {
                ch = _getch();
                if (ch == END_KEY)
                {
                    break;
                }
            }
            clockBeginTime = clockEndTime;
            game.Run(ch);
        }
    }
    game.Close();
    return 0;
}
