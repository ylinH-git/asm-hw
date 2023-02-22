#include <stdio.h>

int main(int argc, char * argv[], char *env[])
{
	int i;
	for(i = 0; i < argc; i++)
	{
		puts(argv[i]);
	}
	for(i = 0; env[i] != NULL; i++)
	{
		puts(env[i]);
	}
	return 0;
}