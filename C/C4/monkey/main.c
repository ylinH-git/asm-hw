#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ����͵�����⣺��һȺ���ӣ�ȥ͵��һ������,
����֮�����ÿ���ʣ�����ӵ�һ��,
��ÿ���ҳ�������֮���и�̰�ĵ�С�ﶼ��͵͵�ٳ�һ������,
���������ķ�ʽ������ÿ�춼���ֵĳ�������
ֱ����ʮ�죬��������������ʱ������ֻʣ��һ��������
�ʣ� �ʼ�ж��ٸ����ӣ�*/

/* ��ʽ a(n-1) = 2 * (a(n) + 1)*/

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
