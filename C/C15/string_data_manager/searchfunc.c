#include "stdafx.h"
#include "strfunc.h"
#include "controller.h"
#include "buf_config.h"

int FindStringIndexMode()
{
	int index;
	int len;
	printf("Please input index of string：\r\n");
	scanf("%4d", &index);
	clean();
	if(CheakNum(index - 1))
	{
		printf("数字输入有误，要求1~4096之间\r\n");
		return 1;
	}
	len = g_szBuffIndexTable[index - 1][1];
	if(!len)
	{
		ShowSearchErr(1);
		return 1;
	}
	else
	{
		int addr = g_szBuffIndexTable[index - 1][0];
		ShowSearchInfo(index, addr, len, &g_szBuff[addr]);
	}
	return 0;
}

int FindStringStrMode()
{
	char str[MAXSTRSIZE];
	int i, strSize, hitFlag = 0;
	printf("Please input a string (less than 100 characters):\r\n");
	sgetc(str, MAXSTRSIZE);
	strSize = MyStrlen(str);
	for (i = 0; i < g_curIndex; i++)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		int k, flag = 0;
		for(k = 0; k < len; k++)
		{
			int currAddr = addr + k;
			if(str[0] == g_szBuff[currAddr])
			{
				int j;
				for(j = 1; j < strSize; j++)
				{
					if(str[j] != g_szBuff[currAddr + j])
					{
						flag = 1;
						break;
					}
				}
				if(!flag)
				{
					hitFlag = 1;
					ShowSearchInfo(i + 1, addr, len, &g_szBuff[addr]);
				}
				flag = 0;
			}
		}
	}
	if(g_curIndex == 0 || hitFlag == 0)
	{
		ShowSearchErr(1);
		return 1;
	}
	return 0;
}

int FindStringAddrMode()
{
	int inputAddr, i, flag = 0;
	printf("Please input an addr (0~4095):\r\n");
	scanf("%4d", &inputAddr);
	clean();
	if(CheakNum(inputAddr))
	{
		printf("数字输入有误，要求0~4095之间\r\n");
		return 1;
	}
	for (i = 0; i < g_curIndex; i++)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		if(inputAddr >= addr && inputAddr <= addr + len)
		{
			flag = 1;
			ShowSearchInfo(i + 1, addr, len, &g_szBuff[addr]);
		}
	}
	if(g_curIndex == 0 || flag == 0)
	{
		ShowSearchErr(1);
		return 1;
	}
	return 0;
}

int FindString()
{
	int cmd = 0;
	ShowSearchMenu();
	scanf("%1d", &cmd);
	clean();
	if(CheakNum(cmd))
	{
		printf("数字输入有误，要求1~3\r\n");
	}
	else 
	{
		switch (cmd)
		{
			case 1:
				return FindStringIndexMode();
			case 2:
				return FindStringStrMode();
			case 3:
				return FindStringAddrMode();
			default:
				printf("数字输入有误，要求1~3之间\r\n");
				return 1;
		}
	}
	return 0;
}


