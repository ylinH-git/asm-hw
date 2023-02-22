#include "stdafx.h"
#include "buf_config.h"
#include "showfunc.h"

double GetPercent(int num)
{
	if(g_total == 0)
	{
		return 0;
	}
	else
	{
		return ((double)num / g_total);
	}
}
void CountChar()
{
	int count[94] = {0};
	int i;
	for(i = 0; i < g_curIndex; i++)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		int j = 0;
		for(j = 0; j < len; j++)
		{
			int charAscii = g_szBuff[addr + j];
			if(charAscii >= 33 && charAscii <= 126)
			{
				count[charAscii - 33]++;
			}
		}
	}
	ShowCount(count, 90);
}