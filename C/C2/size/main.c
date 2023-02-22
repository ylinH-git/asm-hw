#include <stdio.h>
#include <stdlib.h>

int main() {
	int a;
	long b;

	a = 0;
	b = 0;

	printf("%08x, ", &a);
	printf("size of int: %d\n", sizeof(a));
	printf("%08x, ", &b);
	printf("size of long: %d\n", sizeof(b));
	system("pause");
	return 0;
}
