// string_data_manager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "strfunc.h"
#include "showfunc.h"
#include "controller.h"
#include "buf_config.h"

extern char g_szBuff[BUFSIZE] = {0};
extern int g_curIndex = 0;
extern int g_total = 0;

int main(int argc, char* argv[])
{
	while (1)
	{	
		ShowMenu();
		if(!Command())
		{
			break;
		}
		printf("Please input any key to continue!\r\n");
		getchar();
		ClearScreen();
	}
	return 0;
}

