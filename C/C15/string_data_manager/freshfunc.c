#include "stdafx.h"
#include "buf_config.h"

void FreshBuff ()
{
	int i, lastEnd = 0;
	for (i = 0; i < g_curIndex; i++)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		MyStrcpy(&g_szBuff[lastEnd], &g_szBuff[addr]);
		g_szBuffIndexTable[i][0] = lastEnd;
		lastEnd = lastEnd + len + 1;
	}
}