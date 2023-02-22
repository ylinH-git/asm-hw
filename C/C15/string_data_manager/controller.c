#include "stdafx.h"
#include "strfunc.h"
#include "showfunc.h"
#include "writefunc.h"
#include "deletefunc.h"
#include "searchfunc.h"
#include "buf_config.h"
#include "countfunc.h"
#include "changefunc.h"
#include "freshfunc.h"

int CheakString (const char *string)
{
	int i = MyStrlen(string);
	if(i > 0 && i < MAXSTRSIZE)
	{
		return 0;
	}
	return 1;
}

int CheakNum (int num)
{
	if(num >= 0 && num < BUFSIZE)
	{
		return 0;
	}
	return 1;
}

int FindFreeBuf (const char *string)
{
	int i, bufSize = 0;
	int strSize = MyStrlen(string);
	if(g_curIndex == 0)
	{
		return 0;
	}
	for (i = 0; i < g_curIndex; i++)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		int nextAddr =  g_szBuffIndexTable[i + 1][0];
		int nextLen = g_szBuffIndexTable[i + 1][1];
		int endAddr = addr + len + 1;
		int freeFragmentSize = nextAddr - endAddr;
		int freeFragmentAddr = endAddr;
		if(i == 0 && addr != 0)
		{
			freeFragmentSize = addr - 0;
			if(freeFragmentSize > strSize)
			{
				return 0;
			}
		}
		if(nextLen == 0)
		{
			freeFragmentSize = BUFSIZE - endAddr;
		}
		if(freeFragmentSize > strSize)
		{
			return freeFragmentAddr;
		}
	}
	return -1;
}

int Command()
{
	int cmd = 0, ret;
	char input[MAXSTRSIZE] = {0};
	scanf("%1d", &cmd);
	clean();
	if(CheakNum(cmd))
	{
		printf("数字输入有误，要求1~8\r\n");
	}
	else 
	{
		switch (cmd)
		{
			case 1:
				printf("Please input a string (less than 100 characters)!:\r\n");
				sgetc(input, MAXSTRSIZE);
				ret = WriteStrToBuf(input);
				if(!ret)
				{
					printf("Write success!\r\n");
				}
				else
				{
					ShowWriteErr(ret);
				}
				break;
			case 2:
				if(!DeleteString())
				{
					printf("Delete success!\r\n");
				}
				break;
			case 3:
				if(!ChangeString())
				{
					printf("change success!\r\n");
				}
				break;
			case 4:
				FindString();
				break;
			case 5:
				CountChar();
				break;
			case 6:
				ShowBuf();
				break;
			case 7:
				FreshBuff();
				break;
			case 8:
				return 0;
			default:
				printf("数字输入有误，要求1~8之间\r\n");
				break;
		}
	}
	return 1;
}