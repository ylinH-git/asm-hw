// Tetris.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <conio.h>

#include "CGame.h"

int main()
{
  CGame game;
  int speed = 500;
  clock_t clockBeginTime = clock();
  clock_t clockEndTime = 0;
  while (true)
  {
      clockEndTime = clock();
      speed = 500 - ((game.GetGoal() - 1) / 100) * 100;
      speed = speed <= 60 ? 60 : speed;
      game.ShowGame();

      if (clockEndTime - clockBeginTime >= speed)
      {
          clockBeginTime = clockEndTime;
          game.MoveDown();
      }

      if (_kbhit()) {
          char ch = _getch();

          switch (ch)
          {
          case 'a':
              game.MoveLeft();
              break;
          case 's':
              game.MoveDown();
              break;
          case 'w':
              game.MoveUp();
              break;
          case 'd':
              game.MoveRight();
              break;
          case 13:
              game.Restart();
              break;
          case 27:
              game.SetIsExceed(false);
              break;
          case 32:
              game.SetIsExceed(true);
              break;
          default:
              break;
          }
      }
  }
}

