#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[])
{
	int ary[5] = {10, 20, 30, 40, 50};
	int firstP, endP;
	// 利用ary的下标访问取出内存地址0x00400002处的4个字节
	printf("%p\r\n", ((ary[(0x00400000 - (int)&ary) / sizeof(int)]) >> 4 * sizeof(int))
		+ ((ary[(0x00400004 - (int)&ary) / sizeof(int)]) << 4 * sizeof(int)));

	// 利用ary的下标访问本程序入口地址
	firstP = ary[(0x0040003c - (int)&ary) / sizeof(int)];
	endP = ary[(0x00400000 + 40 + firstP - (int)&ary) / sizeof(int)];
	printf("%p\r\n", ary[(0x00400000 + endP - (int)&ary) / sizeof(int)]);
	system("pause");
	return 0;
}