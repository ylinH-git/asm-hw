#include <stdio.h>
#include <stdlib.h>
int main() {
  float fa, fb, fResult;
  printf("请输入两个数：");
  scanf("%f %f", &fa, &fb);

  fResult = fa - fb;

  if(fResult == 0) 
  {
    printf("等于\n");
  } else if (fResult > 0)
  {
    printf("大于\n");
  } else 
  {
    printf("小于\n");
  }

  system("pause");
  return 0;
}
