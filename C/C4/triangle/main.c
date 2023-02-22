#include <stdio.h>
#include <stdlib.h>

#define LEN 10
int main() {
  int i, j;

  for(i = 1; i < LEN; i++) {
    for(j = 0; j < i; j++) {
      printf("%d ", i * (j + 1));
    }
    printf("\n");
  }

  system("pause");
  return 0;
}
