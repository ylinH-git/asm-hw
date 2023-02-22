// find_entry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <locale.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	int imageBase = 0x00400000;
	int *p = (int *) imageBase;
	int temp;
	int entryPoint;
	_tsetlocale(LC_ALL, "chs");

	if(*(short *)p != 'ZM')
	{
		_tprintf("error\r\n");
		return -1;
	}

	temp = p[15];
	p = p + temp / sizeof(int);

	if(*p != 'EP')
	{
		_tprintf("error\r\n");
		return -1;
	}

	entryPoint = p[10];
	_tprintf("entry address: %p\r\n", entryPoint + imageBase);
	_tsystem("pause");
	return 0;
}

