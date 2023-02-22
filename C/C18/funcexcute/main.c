#include <stdio.h>
#include <stdlib.h>

int main (int argc, char argv[])
{
	int ret;
	char szBuf[] = {'\xB8','\x23','\x01','\x00','\x00','\xC3'};
	int (*pfn)() = NULL;
	pfn = (int (__cdecl *)(void))(int)szBuf;
	ret = pfn();
	printf("%d\r\n", ret);
	system("pause");
	return 0;
}