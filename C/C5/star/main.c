#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROW 22
#define COL 40
int main() {
  int i, j;

  for(i = ROW; i >= 0; i--)
  {s
		for(j = 0; j <= COL; j++)
		{
			if ((i == 15 && !(j % 2))
				|| i == 2 * j - 18
				|| i == -2 * j + 62
				|| (i != 0 && !(j % 2) 
						&& (i == - j / 2 + 15 
						|| i == j / 2 - 5))
				)
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\r\n");
  }
  system("pause");
  return 0;
}
