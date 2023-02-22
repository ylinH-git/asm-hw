#include <stdio.h>
#include <stdlib.h>


double power(double x, int n)
{
	if(n > 1){
		n--;
		return x * power(x, n);
	}
	return x;
}
int main(int argc, char *argv[])
{
	printf("%f", power(1.5, 3));
	system("pause");
	return 0;
}