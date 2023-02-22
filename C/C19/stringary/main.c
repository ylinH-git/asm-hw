#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *strAry[3] = {
		"Hello",
		"World",
		"This is a string"
	};
	char charAry[100] = {0};
	int i;
	for(i = 0; i < sizeof(strAry)/ sizeof(strAry[0]); i++)
	{
		strcat(charAry, strAry[i]);
	}
	puts(charAry);
	getchar();
	return 0;
}