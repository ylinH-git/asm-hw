#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// S=¡Ìp(p-a)(p-b)(p-c)
int main() {
	float p, a, b, c, area;

	scanf("%f", &a);
	scanf("%f", &b);
	scanf("%f", &c);

	p = (a + b + c) / 2;
	area = sqrt(p * (p - a) * (p - b) * (p - c));
	printf("area: %f\n", area);
	system("pause");
	return 0;
}
