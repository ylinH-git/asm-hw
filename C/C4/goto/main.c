#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
  int i, n;

  // ģ��do while
  i = 0, n = 0;
  dowhile:
	  n += i;
      i++;

  if(i < 4) {
	goto dowhile;
  }
  printf("i = %d, n = %d\n", i, n);


  // ģ��while
  i = 0, n = 0;
  whileMock: 
	  if(i < 4) {
		n += i;
		i++;
		goto whileMock;
	  }
   printf("i = %d, n = %d\n", i, n);

  // ģ��for
  n = 0, i = 0;
  forMock: 
	  if(i < 4) {
		n += i;
		i++;
		goto forMock;
	  }
  printf("i = %d, n = %d\n", i, n);
  system("pause");
  return 0;
}
