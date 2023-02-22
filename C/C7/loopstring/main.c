#include <stdio.h>
#include <stdlib.h>


void loop(char c) 
{
	if(c != 'a')
	{
		printf("%c", c);
		loop(c-1);
	}
	printf("%c", c);
	
}
int main (int argc, char *argv[])
{
	char c;
	printf("ÇëÊäÈëÒ»¸ö×Ö·û£¨a-z£©:");
	scanf("%[a-z]c", &c);
	loop(c);
	system("pause");
	return 0;
}