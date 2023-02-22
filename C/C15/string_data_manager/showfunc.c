#include "stdafx.h"
#include "buf_config.h"
#include "countfunc.h"

void ClearScreen()
{
	int i;
	for (i = 0; i < 25; i++)
	{
		printf("\r\n");
	}
}

void ShowMenu()
{
	printf(
		"=============================================\r\n"
		"===       输入下列数字编号进入功能模块    ===\r\n"
		"=============    1. 增加字符串   ============\r\n"
		"=============    2. 删除字符串   ============\r\n"
		"=============    3. 修改字符串   ============\r\n"
		"=============    4. 查询字符串   ============\r\n"
		"=============    5. 统计字符     ============\r\n"
		"=============    6. 查看存储区   ============\r\n"
		"=============    7. 整理存储区   ============\r\n"
		"=============    8. 退出         ============\r\n"
		"=============================================\r\n"
	);
}

void ShowSearchMenu()
{
	printf(
		"Please choose a mode to search:\r\n"
		"1. 编号查询（精确查询）:\r\n"
		"2. 字符串查询（支持模糊查询）:\r\n"
		"3. 存储地址查询:\r\n"
	);
}

void ShowDeleteMenu()
{
	printf(
		"Please choose a mode to delete:\r\n"
		"1. 编号模式:\r\n"
		"2. 字符串模式:\r\n"
		);
}

void ShowChangeMenu()
{
	printf(
		"Please choose a mode to change:\r\n"
		"1. 编号模式:\r\n"
		"2. 字符串模式:\r\n"
		);
}


void ShowSearchInfo(int index, int addr, int len, char * str)
{
	int i;
	printf("编号: %d ", index);
	printf("存储地址: %d ", addr);
	printf("字符串长度: %d ", len);
	printf("字符串: ");
	for(i = 0; i < len && str[i] != 0; i++)
	{
		printf("%c", str[i]);
	}
	printf("\r\n");
}

void ShowSearchErr(int code)
{
	switch (code)
	{
		case 1:
			printf("未查询到相关信息 \r\n");
			break;
		default:
			break;
	}
}

void ShowWriteErr(int code)
{
	switch (code)
	{
		case 1:
			printf("字符串输入错误，请重新输入 \r\n");
			break;
		case 2:
			printf("空间不足，存储失败 \r\n");
			break;
		default:
			break;
	}
}

void ShowDeleteErr(int code)
{
	switch (code)
	{
		case 1:
			printf("未查询到此字符串，删除失败 \r\n");
			break;
		default:
			break;
	}
}

void ShowChangeErr(int code)
{
	switch (code)
	{
		case 1:
			printf("未查询到此字符串，修改失败 \r\n");
			break;
		case 2:
			printf("存储区域空间不足，修改失败 \r\n");
			break;
		default:
			break;
	}
}

void ShowCount(int * axsii, int size)
{
	int i;
	printf("注：比例为此字符占所有存储区中有效字符的百分比\r\n");
	for(i = 0; i < size; i++)
	{
		if(!((i + 1) % 6))
		{
			printf(
				"字符 %6c  %6c  %6c  %6c  %6c  %6c\r\n"
				"数量 %6d  %6d  %6d  %6d  %6d  %6d\r\n"
				"比例 %5.2f%%  %5.2f%%  %5.2f%%  %5.2f%%  %5.2f%%  %5.2f%%\r\n"
			, 33 + (i - 5) , 33 + (i - 4), 33 + (i - 3), 33 + (i - 2), 33 + (i - 1), 33 + i,
			axsii[i - 5] , axsii[i - 4], axsii[i - 3], axsii[i - 2], axsii[i - 1], axsii[i],
			GetPercent(axsii[i - 5]), GetPercent(axsii[i - 4]),
			GetPercent(axsii[i - 3]), GetPercent(axsii[i - 2]),
			GetPercent(axsii[i - 1]), GetPercent(axsii[i]));
			printf("\r\n");
		}
	}
}


void ShowBuf()
{
	int i = 0, endAddr = 0, nextAddr = 0;
	double ratio = 0;
	char * tempBuf[BUFSIZE] = {0};
	printf("存储空间===============================================\r\n");
	while(i < g_curIndex)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		int j;
		for (j = addr; j < addr + len + 1; j++)
		{
			tempBuf[j] = 1;
		}
		
		i++;
	}
	
	for (i = 0; i < BUFSIZE; i++)
	{
		if(tempBuf[i] == 1)
		{
			printf("U");
		}
		else
		{
			printf("F");
		}
	}
	printf("\r\n================================================\r\n");
	ratio =  (((double) g_total) / BUFSIZE) * 100;
	printf("空闲区域 %d | 占用区域: %d | 占用比例 %0.2f%%\r\n",
		BUFSIZE - g_total, g_total, ratio);
	printf("存储区索引表：\r\n");
	for(i = 0; i < g_curIndex; i++)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		ShowSearchInfo(i + 1, addr, len, &g_szBuff[addr]);
	}
	printf("\r\n");
}