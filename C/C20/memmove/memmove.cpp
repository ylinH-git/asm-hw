// memmove.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// ��Ŀ���ַ��Դ��ַ�����ص�ʱ��α�֤��ȷ����
// Ŀ���ַ��Դ��ַ��һ���֣���ʱ���ܷ����ڸ���ǰԴ��ַ�е�ֵ�Ѿ�������
void *MyMemmove( void *dest, const void *src, size_t count )
{
	void *ret = dest;
	// û�з����ص������
	if(dest <= src || (char *)dest >= (char *)src + count)
	{
		while(count > 0)
		{
			*(char *)dest = *(char *)src;
			dest = (char *)dest + 1;
			src = (char *)src + 1;
			count--;
		}

 	}
	else 
	{
		dest = (char *)dest + count - 1;
		src = (char *)src + count - 1;
		while(count > 0)
		{
			*(char *)dest = *(char *)src;
			dest = (char *)dest - 1;
			src = (char *)src - 1;
			count--;
		}
	}
	return ret;
}

int main(int argc, char* argv[])
{
	char str1[] = "test string, see what happened";
	char str2[] = "test string, see what happened";
	MyMemmove(str1, str1 + 12, 5);
	printf("%s\r\n", str1);
	MyMemmove(str2 + 6, str2, 13);
	printf("%s\r\n", str2);
	getchar();
	return 0;
}

