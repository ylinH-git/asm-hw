#include <stdio.h>
#include <stdlib.h>
int main() {
  int x, y, z;
  x = -3 + 4 * 5 - 6;
  printf("x = %d\n", x);
  y = -3 + 4 % - 6 / 5;
  printf("y = %d\n", y);
  z = (7 + 6) % 5 / 2;
  printf("z = %d\n", z);

  x = 3;
  y = ++x - 1;
  printf("x = %d, y = %d\n", x, y);
  z = x-- + 1;
  printf("x = %d, z = %d\n", x, z);

  system("pause");
  return 0;
}
