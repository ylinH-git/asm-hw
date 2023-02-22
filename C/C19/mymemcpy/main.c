#include <stdio.h>

void *MyMemcpy( void *dest, const void *src, size_t count )
{
	size_t i;
	for(i = 0; i < count; i++)
	{
		((char *)dest)[i] = ((char*)src)[i];
	}
}


int main()
{
	int arg[5] = {1, 2, 3, 4, 5};
	int arg2[5];
	int i;
	MyMemcpy(arg2, arg, sizeof(arg));
	for(i = 0; i < 5; i++)
	{
		printf("%d ", arg2[i]);
	}
	getchar();
	return 0;
}