#include "stdafx.h"
#include "strfunc.h"
#include "showfunc.h"
#include "buf_config.h"
#include "searchfunc.h"

int GetRestBuf(int index, int addr, int len)
{
	int restBuf = 0;
	if (index == g_curIndex -1)
	{
		restBuf = BUFSIZE - addr - len - 1;
	}
	else
	{
		int nextAddr =  g_szBuffIndexTable[index + 1][0];
		restBuf = nextAddr - addr - len;
	}
	return restBuf;
}

int GetNewAddr(int start, int end, int newStrLen)
{
	int i, newAddr = -1;
	for(i = start; i < end && i < g_curIndex; i++)
	{
		int addr =  g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		int restBuf = GetRestBuf(i, addr, len);
		if(restBuf >= newStrLen)
		{
			newAddr = addr + len;
			break;
		}
	}
	return newAddr;
}

int ChangeStringByIndex(int index)
{
	char newStr[MAXSTRSIZE] = {0};
	int newStrLen, ret;
	int addr =  g_szBuffIndexTable[index][0];
	int len = g_szBuffIndexTable[index][1];
	int restBuf;
	int oldStrlen;
	printf("Please input a string (less than 100 characters)!:\r\n");
	sgetc(newStr, MAXSTRSIZE);
	
	newStrLen = MyStrlen(newStr);
	oldStrlen = MyStrlen(&g_szBuff[addr]);
	restBuf = GetRestBuf(index, addr, len);
	if(restBuf + len > newStrLen)
	{
		MyStrcpy(&g_szBuff[addr], newStr);
		g_szBuffIndexTable[index][1] = newStrLen;
		g_total += newStrLen - oldStrlen;
	} 
	else
	{
		ret = GetNewAddr(index + 1, g_curIndex, newStrLen);
		if(ret == -1)
		{
			ret = GetNewAddr(0, index, newStrLen);
		}
		if(ret == -1)
		{
			ShowChangeErr(2);
			return 2;
		}
		MyStrcpy(&g_szBuff[ret], newStr);
		g_szBuffIndexTable[index][0] = ret;
		g_szBuffIndexTable[index][1] = newStrLen;
		g_total += newStrLen - oldStrlen;
	}
	return 0;
}

int ChangeStringByIndexHaveControl()
{
	int index, len;
	
	printf("Please input index of string：\r\n");
	scanf("%4d", &index);
	clean();
	if(CheakNum(index))
	{
		printf("数字输入有误，要求1~4096\r\n");
		return 1;
	}

	len = g_szBuffIndexTable[index - 1][1];
	if(len == 0)
	{
		ShowChangeErr(1);
		return 1;
	}
	return ChangeStringByIndex(index - 1);
}
int ChangeString()
{
	int cmd = 0;
	ShowChangeMenu();
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
			return ChangeStringByIndexHaveControl();
		case 2:
			ret = FindStringStrMode();
			if(ret)
			{
				return 1;
			}
			return ChangeStringByIndexHaveControl();
		default:
			printf("数字输入有误，要求1~2之间\r\n");
			return 1;
		}
	}
	return 0;
}