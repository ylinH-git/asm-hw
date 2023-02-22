#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX 20000
#define LimitInt 2000000000

int main() {
	int arr[77340] = {1}, carry = 0, cnt = 1, i, j, num, base;
	clock_t start, end;
	double duration;

	start = clock();
	base = LimitInt / MAX;
	for(i = 0; i < MAX; i++) {
		for(j = 0; j < cnt; j++) {
			num = arr[j] * (i + 1) + carry;
			arr[j] = num % base;
			carry = num / base;
		}

		while (carry)
		{
			arr[cnt++] = carry % base;
			carry /= base;
		}
	}
	end = clock();
	for(i = cnt - 1; i >= 0; i--){
		printf("%d", arr[i]);
	}
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\n%f seconds\n", duration);
	return 0;
}
