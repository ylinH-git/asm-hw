#include <stdio.h>
#include <stdlib.h>

int main() {
  int iArr[5] = {0}, iVal, i, j;

  printf("������һ��������5λ����������");
  scanf("%5d", &iVal);

  i = 0;
  while(iVal > 0)
  {
    iArr[i++] = iVal % 10;
    iVal /= 10;
  }
  
  printf("����Ϊһ��%dλ��\r\n", i);

  // ����
  for(j = i - 1; j >= 0; j--)
  {
    printf("%d", iArr[j]);
  }
  printf("\r\n");
  // ����
  for(j = 0; j < i; j++)
  {
    printf("%d", iArr[j]);
  }
  printf("\r\n");
  system("pause");
  return 0;
}
