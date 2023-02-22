// variant.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum eType 
{
	T_INT,
	T_UNSIGNED_INT,
	T_CHAR,
	T_UNSIGNED_CHAR,
	T_FLOAT,
	T_DOUBLE,
};
struct tagVarient
{
	enum eType type;
	union
	{
		int nInt;
		unsigned uInt;
		char cChar;
		unsigned char uChar;
		float fFloat;
		double dbBouble;
	} uValue;
};

void set(struct tagVarient* variant, void *v,enum eType e)
{
	switch(e)
	{
	case T_INT: 
		variant->type = T_INT;
		variant->uValue.nInt = *(int *)v;
		break;
	case T_UNSIGNED_INT: 
		variant->type = T_UNSIGNED_INT;
		variant->uValue.uInt = *(unsigned int *)v;
		break;
	case T_CHAR: 
		variant->type = T_CHAR;
		variant->uValue.cChar = *(char *)v;
		break;
	case T_UNSIGNED_CHAR: 
		variant->type = T_UNSIGNED_CHAR;
		variant->uValue.uChar = *(unsigned char *)v;
		break;
	case T_FLOAT: 
		variant->type = T_FLOAT;
		variant->uValue.fFloat = *(float *)v;
		break;
	case T_DOUBLE: 
		variant->type = T_DOUBLE;
		variant->uValue.dbBouble = *(double *)v;
		break;
	default:
		printf("wrong type!\r\n");
		break;
	}
};

void *get(struct tagVarient* variant)
{
	switch(variant->type)
	{
	case T_INT: 
		return (void *)&(variant->uValue.nInt);
	case T_UNSIGNED_INT: 
		return (void *)&(variant->uValue.uInt);
	case T_CHAR: 
		return (void *)&(variant->uValue.cChar);
	case T_UNSIGNED_CHAR: 
		return (void *)&(variant->uValue.uChar);
	case T_FLOAT: 
		return (void *)&(variant->uValue.fFloat);
	case T_DOUBLE: 
		return (void *)&(variant->uValue.dbBouble);
	default:
		printf("wrong type!\r\n");
		break;
	}
};


int main(int argc, char* argv[])
{
	struct tagVarient vInt;
	int nInt = 10;
	set(&vInt, &nInt, T_INT);
	int ntest1 = *(int *)get(&vInt);
	printf("%d, %d\r\n", vInt.type, vInt.uValue.nInt);
	printf("%d\r\n", ntest1);
	
	struct tagVarient vUInt;
	unsigned int uInt = 11;
	set(&vUInt, &uInt, T_UNSIGNED_INT);
	unsigned int ntest2 = *(unsigned int *)get(&vUInt);
	printf("%d, %u\r\n", vUInt.type, vUInt.uValue.uInt);
	printf("%u\r\n", ntest2);

	struct tagVarient vChar;
	char cChar = 't';
	set(&vChar, &cChar, T_CHAR);
	char ntest3 = *(char *)get(&vChar);
	printf("%d, %c\r\n", vChar.type, vChar.uValue.cChar);
	printf("%c\r\n", ntest3);

	struct tagVarient vUChar;
	unsigned char uChar = 'u';
	set(&vUChar, &uChar, T_UNSIGNED_CHAR);
	unsigned char ntest4 = *(unsigned char *)get(&vUChar);
	printf("%d, %c\r\n", vUChar.type, vUChar.uValue.uChar);
	printf("%c\r\n", ntest4);

	struct tagVarient vFloat;
	float fFloat = 12.00f;
	set(&vFloat, &fFloat, T_FLOAT);
	float ntest5= *(float *)get(&vFloat);
	printf("%d, %f\r\n", vFloat.type, vFloat.uValue.fFloat);
	printf("%f\r\n", ntest5);

	struct tagVarient vDouble;
	double dbDouble = 13.00f;
	set(&vDouble, &dbDouble, T_DOUBLE);
	double ntest6= *(double *)get(&vDouble);
	printf("%d, %f\r\n", vDouble.type, vDouble.uValue.dbBouble);
	printf("%f\r\n", ntest6);
	getchar();
	return 0;
}

