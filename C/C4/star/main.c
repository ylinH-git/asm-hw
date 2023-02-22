#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROW 13
#define COL 7
int main() {
  int r, c, half;
  half = COL / 2 + 1;
  r = 0;
  while (r < ROW)
  {
    if(!((r + 1) % 2))
    {
      printf("\r\n");
      printf("\r\n");
    }
    else if(r == 0 || r == ROW - 1)
    {
      for (c = 0; c < COL; c++)
      {
        if(c != half - 1)
        {
          printf(" ");
        }
        else
        {
          printf("*");
        }
      }
    }
    else if (r < 7)
    {
      int s = r / 2;
      for (c = 0; c < COL; c++)
      {
        if (c == half - s - 1 || c == half + s - 1)
        {
          printf("*");
        } 
        else
        {
          printf(" ");
        }
      }
    }
    else
    {
      int s = (r - 6) / 2;
      for (c = 0; c < COL; c++)
      {
        if (c == COL - s - 1 || c == 0 + s)
        {
          printf("*");
        } 
        else
        {
          printf(" ");
        }
      }
    }
    r++;
  }
  system("pause");
  return 0;
}
