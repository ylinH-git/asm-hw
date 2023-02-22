#include "stdafx.h"
#include "strfunc.h"
#include "controller.h"
#include "searchfunc.h"
#include "showfunc.h"
#include "buf_config.h"

void ReArrangeIndexTable()
{
	int i;
	for(i = 0; i < g_curIndex - 1; i++)
	{
		if(g_szBuffIndexTable[i][1] == 0)
		{
			g_szBuffIndexTable[i][0] = g_szBuffIndexTable[i + 1][0];
			g_szBuffIndexTable[i][1] = g_szBuffIndexTable[i + 1][1];
			g_szBuffIndexTable[i + 1][0] = 0;
			g_szBuffIndexTable[i + 1][1] = 0;
		}
	}
}

int DeleteStringByIndex(int index)
{
	int addr =  g_szBuffIndexTable[index][0];
	int len = g_szBuffIndexTable[index][1];

	if(len == 0)
	{
		ShowDeleteErr(1);
		return 1;
	}

	g_szBuffIndexTable[index][0] = 0;
	g_szBuffIndexTable[index][1] = 0;
	ReArrangeIndexTable();
	g_total -= (len + 1);
	g_curIndex--;
	return 0;
}

int DeleteStringByIndexHaveControl()
{
	int index;

	printf("Please input index of string：\r\n");
	scanf("%4d", &index);
	clean();
	if(CheakNum(index))
	{
		printf("数字输入有误，要求1~4096\r\n");
		return 1;
	}
	return DeleteStringByIndex(index - 1);
}


int DeleteString()
{
	int cmd = 0;
	ShowDeleteMenu();
	scanf("%1d", &cmd);
	clean();
	if(CheakNum(cmd))
	{
		printf("数字输入有误，要求1~2\r\n");
	}
	else 
	{
		int ret;
		switch (cmd)
		{
		case 1:
			return DeleteStringByIndexHaveControl();
		case 2:
			ret = FindStringStrMode();
			if(ret)
			{
				return 1;
			}
			return DeleteStringByIndexHaveControl();
		default:
			printf("数字输入有误，要求1~2之间\r\n");
			return 1;
		}
	}
	return 0;
}