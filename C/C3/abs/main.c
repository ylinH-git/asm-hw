#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main() {
  int ia, ib, ic;
  printf("����������������");
  scanf("%d %d", &ia, &ib);

  ic = ia + abs(ib);
  printf("%d + |%d| = %d\n", ia, ib, ic);

  system("pause");
  return 0;
}
