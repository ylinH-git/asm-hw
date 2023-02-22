#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int __cdecl ShowChangedChar(char ch){
	ch = ch + (rand() % 20);
	printf("%c", ch);
	system("pause");
	return 0;
}

int __cdecl Draw(char ch ,int count){
	char c = ch + (rand() % 10);
	int i;
	srand( (unsigned)time( NULL ) );
	for(i = 0; i < count; i++)
	{
		ShowChangedChar(c);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int i = 10;
	int c = 'a';
	printf("Address is %08x\r\n", &i);
	Draw(c,i);
	return 0;
}