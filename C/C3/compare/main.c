#include <stdio.h>
#include <stdlib.h>
int main() {
  float fa, fb, fResult;
  printf("��������������");
  scanf("%f %f", &fa, &fb);

  fResult = fa - fb;

  if(fResult == 0) 
  {
    printf("����\n");
  } else if (fResult > 0)
  {
    printf("����\n");
  } else 
  {
    printf("С��\n");
  }

  system("pause");
  return 0;
}
