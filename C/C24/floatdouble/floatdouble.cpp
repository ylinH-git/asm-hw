// floatdouble.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct tagDouble
{
	int nSign:1;
	int nESite: 11;
	int nHData: 20;
	int nLData: 32;
};

int main(int argc, char* argv[])
{
	float fPi = 3.14f;
	int *nPi = (int *)&fPi;
	double dbPi = -3.14f;
	int *nLPi = (int *)&dbPi;
	int *nHPi = (int *)&dbPi + 1;
	int nSign, nESite, nData;
	int i;
	struct tagDouble sDouble;
	printf("3.14(float): %p\r\n", *nPi);
	nSign = *nPi & 0x80000000;
	nESite = *nPi & 0x7f800000;
	nData = *nPi & 0x000fffff;
	printf("符号位: %p\r\n", nSign);
	printf("指数位: %p\r\n", nESite);
	printf("数据位: %p\r\n", nData);

	printf("-3.14(double): %p%p\r\n", *nHPi,*nLPi);
	sDouble.nSign = *nHPi >> 31;
	sDouble.nESite = *nHPi >> 20;
	sDouble.nHData = *nHPi;
	sDouble.nLData = *nLPi;
	printf("符号位: %p\r\n", sDouble.nSign);
	printf("符号位（二进制）: ");
	for(i = 0; i > 0;i++)
	{
		printf("%c", (unsigned int)(sDouble.nSign >> i) % 2 + '0');
	}
	printf("\r\n");
	printf("指数位: %p\r\n", sDouble.nESite);
	printf("指数位（二进制）: ");
	for(i = 10; i >= 0;i--)
	{
		printf("%c", (unsigned int)(sDouble.nESite >> i) % 2 + '0');
	}
	printf("\r\n");
	printf("数据位: %p%p\r\n", sDouble.nHData, sDouble.nLData);
	printf("数据位（二进制）: ");
	for(i = 19; i >= 0;i--)
	{
		printf("%c", (unsigned int)(sDouble.nHData >> i) % 2 + '0');
	}
	for(i = 31; i >= 0;i--)
	{
		printf("%c", (unsigned int)(sDouble.nLData >> i) % 2 + '0');
	}
	printf("\r\n");
	getchar();
	return 0;
}

