#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv)
{
	char cmd[1000] = "printftostream.exe ";
	int i;
	for(i = 1; i< argc; i++)
	{
		strcat(cmd, argv);
	}
	strcat(cmd, " > 1.txt");
	system(cmd);
	return 0;
}