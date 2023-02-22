#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char cmd[510] = "iexplore ";
	char *url = argv[1];
	if(argc <= 1)
	{
		return -1;
	}
	strcat(cmd, url);
	system(cmd);
	getchar();
	return 0;
}
