#ifndef BUFCONFIG_H
#define BUFCONFIG_H

#define BUFSIZE 4096
#define MAXSTRSIZE 100
char g_szBuff[BUFSIZE];
int g_szBuffIndexTable[BUFSIZE][2];
int g_curIndex;
int g_total;
#endif