#include "stdafx.h"
#include "strfunc.h"
#include "buf_config.h"
#include "controller.h"

void SortIndexTable()
{
	int temp[2] = {0};
	int i, j;
	for(i = 0; i < g_curIndex; i++) {
		for(j = i; j < g_curIndex; j++) {
			int addrI = g_szBuffIndexTable[i][0];
			int addrJ = g_szBuffIndexTable[j][0];
			if(addrI > addrJ) {
				temp[0] = addrI;
				temp[1] = g_szBuffIndexTable[i][1];
				g_szBuffIndexTable[i][0] = addrJ;
				g_szBuffIndexTable[i][1] = g_szBuffIndexTable[j][1];
				g_szBuffIndexTable[j][0] = temp[0];
				g_szBuffIndexTable[j][1] = temp[1];
			}
		}
	}
}
int WriteStrToBuf (const char *string)
{
	int addr;
	if(CheakString(string))
	{
		return 1;	
	}
	
	addr = FindFreeBuf(string);
	if(addr == -1)
	{
		return 2;
	}
	else
	{
		MyStrcpy(&g_szBuff[addr], string);
		g_szBuffIndexTable[g_curIndex][0] = addr;
		g_szBuffIndexTable[g_curIndex][1] = MyStrlen(string);
		g_total += g_szBuffIndexTable[g_curIndex][1] + 1;
		g_curIndex++;
		SortIndexTable();
	}
	
	return 0;
}
