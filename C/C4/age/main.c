#include <stdio.h>
#include <stdlib.h>

int getDays(int year, int month, int date) {
  int i, days;
  days = date;
  for (i = 0; i < month; i++)
  {
    int monthMaxDay;
    switch(i) {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        monthMaxDay = 31;
        break;
      case 2:
        monthMaxDay = year % 4 ? 28 : 29;
        break;
      default:
        monthMaxDay = 30;
        break;
    }
    days += monthMaxDay;
  }
  return days;
}

int main() {
  int year1, month1, date1, year2, month2, date2, n, days, days1, days2;
  
  printf("���������������(��ʽ��yyyy mm dd)��");
  scanf("%4d %2d %2d", &year1, &month1, &date1);

  printf("���������������(��ʽ��yyyy mm dd)��");
  scanf("%4d %2d %2d", &year2, &month2, &date2);

  for(n = year1, days = 0; n < year2; n++) {
    if(!(n % 4)) 
    {
      days += 366;
    }
    else
    {
      days += 365;
    }
  }
  days1 = getDays(year1, month1, date1);
  days2 = getDays(year2, month2, date2);

  days = days + days2 - days1;
  
  printf("������������%d\n", days);
  if(days % 23 >= 0 && days % 23 <= 11)
  {
    printf("����״̬: �����߳���\n");
  }
  else
  {
    printf("����״̬: �����ͳ���\n");
  }

  if(days % 28 >= 0 && days % 28 <= 14)
  {
    printf("����״̬: �����߳���\n");
  }
  else
  {
    printf("����״̬: �����ͳ���\n");
  }

  if(days % 33 >= 0 && days % 33 <= 16)
  {
    printf("����״̬: �����߳���\n");
  }
  else
  {
    printf("����״̬: �����ͳ���\n");
  }
  system("pause");
  return 0;
}
