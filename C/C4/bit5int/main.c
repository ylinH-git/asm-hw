#include <stdio.h>
#include <stdlib.h>

int main() {
  int iArr[5] = {0}, iVal, i, j;

  printf("请输入一个不多于5位的正整数：");
  scanf("%5d", &iVal);

  i = 0;
  while(iVal > 0)
  {
    iArr[i++] = iVal % 10;
    iVal /= 10;
  }
  
  printf("此数为一个%d位数\r\n", i);

  // 正序
  for(j = i - 1; j >= 0; j--)
  {
    printf("%d", iArr[j]);
  }
  printf("\r\n");
  // 逆序
  for(j = 0; j < i; j++)
  {
    printf("%d", iArr[j]);
  }
  printf("\r\n");
  system("pause");
  return 0;
}
