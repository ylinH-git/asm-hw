#include <stdio.h>
#include <stdlib.h>

#define LEN 10
int main() {
  float fArr[LEN], ftemp;
  int i, j;
  printf("ÇëÊäÈë10¸öÊı×Ö£º");

  for (i = 0; i < LEN; i++) 
  {
    scanf("%f", &fArr[i]);
  }

  for(i = 0; i < LEN; i++) {
    for(j = i; j < LEN; j++) {
      if(fArr[i] > fArr[j]) {
        ftemp = fArr[i];
        fArr[i] = fArr[j];
        fArr[j] = ftemp;
      }
    }
  }

   printf("%.1f ", fArr[0] + fArr[LEN - 1]);
  system("pause");
  return 0;
}
