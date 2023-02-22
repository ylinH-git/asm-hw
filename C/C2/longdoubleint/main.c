#include <stdio.h>
#include <stdlib.h>
int main() {
	long double n1 = 36;
	long int n2 = 36;
	double long n3 = 36;
	printf("long double: %08x, ", &n1);
	printf("sizeof: %d\n", sizeof(n1));
	printf("long int: %08x, ", &n2);
	printf("sizeof: %d\n", sizeof(n2));
	printf("double long %08x, ", &n3);
	printf("sizeof: %d\n", sizeof(n3));
	system("pause");
	return 0;
}
