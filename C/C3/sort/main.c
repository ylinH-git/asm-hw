#include <stdio.h>
#include <stdlib.h>
int main() {
	int iArr[3], itemp, i, j;
	printf("请输入三个整数：");
  scanf("%d %d %d", &iArr[0], &iArr[1], &iArr[2]);

  for(i = 0; i < 3; i++) {
    for(j = i; j < 3; j++) {
      if(iArr[i] > iArr[j]) {
        itemp = iArr[i];
        iArr[i] = iArr[j];
        iArr[j] = itemp;
      }
    }
  }

  for(i = 0; i < 3; i++) {
    printf("%d ", iArr[i]);
  }
	system("pause");
	return 0;
}
