// copy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

int FileCopy(const char *source, const char *target)
{
	FILE *fSource = fopen(source, "r+b");
	FILE *fTarget = NULL;
	char  *pBuf[0x1000] = {0};
	unsigned int uReadSize = 0;
	if(fSource == NULL) {
		printf("复制文件打开失败!\r\n");
		return 1;
	}

	fTarget = fopen(target, "w+b");
	if(fTarget == NULL)
	{
		printf("复制失败!\r\n");
		return 1;
	}

	while(!feof(fSource))
	{
		uReadSize = fread(pBuf, 1, sizeof(pBuf), fSource);
		fwrite(pBuf, uReadSize, 1, fTarget);
	}
	return 0;
}
int main(int argc, char* argv[])
{
	int nRet = FileCopy(argv[1], argv[2]);
	if(!nRet)
	{
		printf("复制成功!\r\n");
	}
	else 
	{
		printf("复制失败!\r\n");
	}

	return nRet;
}

