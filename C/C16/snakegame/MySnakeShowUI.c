#include <stdio.h>
#include <stdlib.h>
#include "unicode.h"
#include "MySnakeControl.h"

int ShowUI(int res)
{
	int i, j;
	_tsystem(_L("cls"));
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (g_szMaxBuff[i][j] == WALL)
			{
				_tprintf(_L("��"));
			}
			else if (g_szMaxBuff[i][j] == SNAKE)
			{
				_tprintf(_L("��"));
			}
			else if (g_szMaxBuff[i][j] == FOOD)
			{
				_tprintf(_L("��"));
			}
			else
			{
				_tprintf(_L("  "));
			}
		}
		_tprintf(_L("\r\n"));
	}
	_tprintf(_L("��ǰ�ܻ��֣�%d "), g_nSnakeCount - 1);
	_tprintf(_L("nDirection: %d\r\n"), nDirection);
	if(res == 1)
	{
		_tprintf(_L("You are win!\r\n"));
		_tprintf(_L("������ո����¿�ʼ��Ϸ��esc�˳�"));
	}
	else if(res == 2)
	{
		_tprintf(_L("GameOver!\r\n"));
		_tprintf(_L("������ո����¿�ʼ��Ϸ��esc�˳�"));
	}
	return 0;
}