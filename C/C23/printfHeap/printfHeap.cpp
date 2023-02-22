// printfHeap.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <crtdbg.h>


#ifdef _DEBUG
#define malloc(n) _malloc_dbg(n, _NORMAL_BLOCK,__FILE__,__LINE__)
#endif


void ErrorProc()
{
	puts("error");
}


struct tagHeap
{
	int *pLastArr;
	int *pNextArr;
	char *pFile;
	int nCodeLine;
	int nHeapLenth;
	int nHeapType;
	int nHeapIndex;
	int nHeapCeil;
	void* vPTrueArea;
	int nHeapFloor;
};
int main(int argc, char *argv[], char *envp[])
{
	struct tagHeap tHeap = {
		NULL,
		NULL,
		NULL,
		0,
		0,
		0,
		0,
		0,
		NULL,
		0,
	};
	char *psz = NULL;
	int *vp = NULL;
	psz = (char *) malloc(sizeof(char[20]));
	if(psz == NULL)
	{
		ErrorProc();
		goto EXIT_PROC;
	}
	

	vp = (int *)(psz - 0x20);
	tHeap.pLastArr = (int *)*vp;
	tHeap.pNextArr = (int *)*(vp + 1);
	tHeap.nHeapIndex = *(vp + 6);
	while(tHeap.nHeapIndex > 1)
	{
		vp = tHeap.pLastArr;
		tHeap.pLastArr = (int *)*vp;
		tHeap.pNextArr = (int *)*(vp + 1);
		tHeap.nHeapIndex = *(vp + 6);
	}

	while(tHeap.pNextArr != 0)
	{
		int j = 0;
		tHeap.pLastArr = (int *)*vp;
		printf("上个堆地址: %p\r\n", tHeap.pLastArr);
		tHeap.pNextArr = (int *)*(++vp);
		printf("下个堆地址: %p\r\n", tHeap.pNextArr);
		tHeap.pFile = (char *)*(++vp);
		printf("代码文件: %s\r\n", tHeap.pFile);
		tHeap.nCodeLine =  *(++vp);
		printf("代码行: %d\r\n", tHeap.nCodeLine);
		tHeap.nHeapLenth = *(++vp);
		printf("堆长度: %d\r\n", tHeap.nHeapLenth);
		tHeap.nHeapType = *(++vp);
		printf("堆类型: %d\r\n", tHeap.nHeapType);
		tHeap.nHeapIndex = *(++vp);
		printf("堆索引: %d\r\n", tHeap.nHeapIndex);
		tHeap.nHeapCeil = *(++vp);
		printf("堆上溢代码: %p\r\n", tHeap.nHeapCeil);
		tHeap.vPTrueArea= ++vp;
		printf("实际空间: ");
		for(j = 0; j < tHeap.nHeapLenth; j++)
		{
			printf("%02x ", *((unsigned char *)tHeap.vPTrueArea + j));
			if(j > 7)
			{
				printf("...");
				break;
			}
		}
		printf("\r\n");
		tHeap.nHeapFloor = *(int*)((unsigned char *)vp + tHeap.nHeapLenth);
		printf("堆下溢代码: %p\r\n", tHeap.nHeapFloor);
		printf("\r\n---------------------------------------------------\r\n");
		vp = tHeap.pNextArr;
	}
	
EXIT_PROC:
	if (psz != NULL)
	{
		free(psz);
		psz = NULL;
	}
	getchar();
	return 0;
}
