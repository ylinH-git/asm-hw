#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 猴子偷桃问题：有一群猴子，去偷了一堆桃子,
商量之后决定每天吃剩余桃子的一半,
当每天大家吃完桃子之后，有个贪心的小猴都会偷偷再吃一个桃子,
按照这样的方式猴子们每天都快乐的吃着桃子
直到第十天，当大家再想吃桃子时，发现只剩下一个桃子了
问： 最开始有多少个桃子？*/

/* 公式 a(n-1) = 2 * (a(n) + 1)*/

int lastDayFood(int day, int num)
{
  if(day == 1)
  {
    return num;
  }
  else
  {
	day--;
    num = 2 * (num + 1);
    return lastDayFood(day, num);
  }
}

int main() {
  int total = lastDayFood(10, 1);
  printf("%d", total);
  system("pause");
  return 0;
}
