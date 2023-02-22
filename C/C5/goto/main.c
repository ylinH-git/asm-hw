#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
  int i, n;

  // Ä£Äâdo while
  // 
  // do {
  //    n += i;
  //    i++;
  // } while(i < 4)
  i = 0, n = 0;
dowhile:
  n += i;
  i++;

  if(i < 4) {
	goto dowhile;
  }
  printf("i = %d, n = %d\n", i, n);


  // Ä£Äâwhile
  // while(i < 4)
  // {
  //    n += i;
  //    i++;
  // }
  i = 0, n = 0;
whileBegin:
  if(i >= 4) {
	goto whileEnd;
  }
  n += i;
  i++;
  goto whileBegin;
whileEnd:
  printf("i = %d, n = %d\n", i, n);

  // Ä£Äâfor
  // for(i = 0; i < 4; i++)
  // {
  //    n += i;
  // }

  i = 0, n = 0;
  goto FOR_CMP;
FOR_SETP:
  i++;
FOR_CMP:
  if(i >= 4) {
	  goto FOR_END;
  }
  n += i;
  goto FOR_SETP;
FOR_END:
  printf("i = %d, n = %d\n", i, n);
  system("pause");
  return 0;
}
