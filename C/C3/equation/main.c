#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main() {
  float fa, fb, fc, fResult1, fResult2, hasResult;
  printf("��������������");
  scanf("%f %f %f", &fa, &fb, &fc);
  
  // b^2 - 4ac > 0 ���н�
  // double������ھ������
  hasResult = (float)pow(fb, 2) - 4 * fa * fc;
  if(hasResult < 0) {
    printf("�޽�\n");
  } else {
    fResult1 = (-fb + sqrt(hasResult)) / (2 * fa);
    fResult2 = (-fb - sqrt(hasResult)) / (2 * fa);
    if(fResult1 == fResult2) {
      printf("x = %.2f\n", fResult1);
    } else {
      printf("x = %.2f or %.2f\n", fResult1, fResult2);
    }
  }
  
  system("pause");
  return 0;
}

