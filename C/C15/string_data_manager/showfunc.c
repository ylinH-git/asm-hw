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
		"===       �����������ֱ�Ž��빦��ģ��    ===\r\n"
		"=============    1. �����ַ���   ============\r\n"
		"=============    2. ɾ���ַ���   ============\r\n"
		"=============    3. �޸��ַ���   ============\r\n"
		"=============    4. ��ѯ�ַ���   ============\r\n"
		"=============    5. ͳ���ַ�     ============\r\n"
		"=============    6. �鿴�洢��   ============\r\n"
		"=============    7. ����洢��   ============\r\n"
		"=============    8. �˳�         ============\r\n"
		"=============================================\r\n"
	);
}

void ShowSearchMenu()
{
	printf(
		"Please choose a mode to search:\r\n"
		"1. ��Ų�ѯ����ȷ��ѯ��:\r\n"
		"2. �ַ�����ѯ��֧��ģ����ѯ��:\r\n"
		"3. �洢��ַ��ѯ:\r\n"
	);
}

void ShowDeleteMenu()
{
	printf(
		"Please choose a mode to delete:\r\n"
		"1. ���ģʽ:\r\n"
		"2. �ַ���ģʽ:\r\n"
		);
}

void ShowChangeMenu()
{
	printf(
		"Please choose a mode to change:\r\n"
		"1. ���ģʽ:\r\n"
		"2. �ַ���ģʽ:\r\n"
		);
}


void ShowSearchInfo(int index, int addr, int len, char * str)
{
	int i;
	printf("���: %d ", index);
	printf("�洢��ַ: %d ", addr);
	printf("�ַ�������: %d ", len);
	printf("�ַ���: ");
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
			printf("δ��ѯ�������Ϣ \r\n");
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
			printf("�ַ�������������������� \r\n");
			break;
		case 2:
			printf("�ռ䲻�㣬�洢ʧ�� \r\n");
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
			printf("δ��ѯ�����ַ�����ɾ��ʧ�� \r\n");
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
			printf("δ��ѯ�����ַ������޸�ʧ�� \r\n");
			break;
		case 2:
			printf("�洢����ռ䲻�㣬�޸�ʧ�� \r\n");
			break;
		default:
			break;
	}
}

void ShowCount(int * axsii, int size)
{
	int i;
	printf("ע������Ϊ���ַ�ռ���д洢������Ч�ַ��İٷֱ�\r\n");
	for(i = 0; i < size; i++)
	{
		if(!((i + 1) % 6))
		{
			printf(
				"�ַ� %6c  %6c  %6c  %6c  %6c  %6c\r\n"
				"���� %6d  %6d  %6d  %6d  %6d  %6d\r\n"
				"���� %5.2f%%  %5.2f%%  %5.2f%%  %5.2f%%  %5.2f%%  %5.2f%%\r\n"
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
	printf("�洢�ռ�===============================================\r\n");
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
	printf("�������� %d | ռ������: %d | ռ�ñ��� %0.2f%%\r\n",
		BUFSIZE - g_total, g_total, ratio);
	printf("�洢��������\r\n");
	for(i = 0; i < g_curIndex; i++)
	{
		int addr = g_szBuffIndexTable[i][0];
		int len = g_szBuffIndexTable[i][1];
		ShowSearchInfo(i + 1, addr, len, &g_szBuff[addr]);
	}
	printf("\r\n");
}