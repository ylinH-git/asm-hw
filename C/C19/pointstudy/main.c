#include <stdio.h>

int main(int argc, char *argv[]) {
	int ary[2][3]= {
		{10, 20, 30},
		{50, 60, 70}
	};
	int (*p2)[3] = ary;

	// ary 得到一维数组int[3]类型的地址指针
	// sizeof 得到 2 * 3 * 4 = 24 输出18
	// ary + 1 得到一维数组int[3]类型的地址
	printf("%08x\r\n", ary);
	printf("%08x\r\n", sizeof(ary));
	printf("%08x\r\n", ary+1);
	
	// p2 得到 int[3]类型的地址指针
	// int[3]类型是一维数组
	// p2 + 1 得到一维数组int[3]类型的地址指针
	printf("%08x\r\n", p2);
	printf("%08x\r\n", sizeof(p2));
	printf("%08x\r\n", p2+1);
	
	// p2 得到 int[3]类型的地址指针
	// int[3]类型是一维数组
	// *p2 是int[3]类型，得到int类型的指针
	// *p2+1 得到 int类型的地址指针
	printf("%08x\r\n", *p2);
	printf("%08x\r\n", sizeof(*p2));
	printf("%08x\r\n", *p2+1);
	
	// p2 得到 int[3]类型的地址指针
	// int[3]类型是一维数组
	// *p2 得到 int类型的地址指针
	// **p2 得到 int类型
	// **p2 + 1 得到int类型
	printf("%08x\r\n", **p2);
	printf("%08x\r\n", sizeof(**p2));
	printf("%08x\r\n", **p2+1);
	
	// p2 得到 int[3]类型的地址指针
	// int[3]类型是一维数组
	// p2 + 1 得到 int[3]类型的地址指针
	// (p2 + 1)[1] 得到 int[3]类型
	printf("%08x\r\n", (p2 + 1)[1]);
	
	// p2 得到 int[3]类型的地址指针
	// int[3]类型是一维数组
	// (*p2) 得到 int类型的地址指针
	// (*p2)[1] 得到int类型
	// (*p2)[1] + 1得到int类型
	printf("%08x\r\n", (*p2)[1] + 1);
	
	// p2 得到 int[3]类型的地址指针
	// int[3]类型是一维数组
	// p2 + 1 得到 int[3]类型的地址指针
	// (*(p2 + 1)) 得到 int类型地址指针
	// (*(p2 + 1))[1] 得到int类型
	printf("%08x\r\n", (*(p2 + 1))[1]);
	getchar();
	return 0;
}