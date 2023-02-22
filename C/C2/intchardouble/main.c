#include <stdio.h>
#include <stdlib.h>
int main() {
	int n1 = 36;
	char n2 = 36;
	double n3 = 36;
	printf("int: %08x\n", &n1);
	printf("char: %08x\n", &n2);
	printf("double %08x\n", &n3);
	system("pause");
	return 0;
}
