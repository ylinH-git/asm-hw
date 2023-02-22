#include <stdio.h>

int main(int argc, char *argv[]) {
	int ary[2][3]= {
		{10, 20, 30},
		{50, 60, 70}
	};
	int (*p2)[3] = ary;

	// ary �õ�һά����int[3]���͵ĵ�ַָ��
	// sizeof �õ� 2 * 3 * 4 = 24 ���18
	// ary + 1 �õ�һά����int[3]���͵ĵ�ַ
	printf("%08x\r\n", ary);
	printf("%08x\r\n", sizeof(ary));
	printf("%08x\r\n", ary+1);
	
	// p2 �õ� int[3]���͵ĵ�ַָ��
	// int[3]������һά����
	// p2 + 1 �õ�һά����int[3]���͵ĵ�ַָ��
	printf("%08x\r\n", p2);
	printf("%08x\r\n", sizeof(p2));
	printf("%08x\r\n", p2+1);
	
	// p2 �õ� int[3]���͵ĵ�ַָ��
	// int[3]������һά����
	// *p2 ��int[3]���ͣ��õ�int���͵�ָ��
	// *p2+1 �õ� int���͵ĵ�ַָ��
	printf("%08x\r\n", *p2);
	printf("%08x\r\n", sizeof(*p2));
	printf("%08x\r\n", *p2+1);
	
	// p2 �õ� int[3]���͵ĵ�ַָ��
	// int[3]������һά����
	// *p2 �õ� int���͵ĵ�ַָ��
	// **p2 �õ� int����
	// **p2 + 1 �õ�int����
	printf("%08x\r\n", **p2);
	printf("%08x\r\n", sizeof(**p2));
	printf("%08x\r\n", **p2+1);
	
	// p2 �õ� int[3]���͵ĵ�ַָ��
	// int[3]������һά����
	// p2 + 1 �õ� int[3]���͵ĵ�ַָ��
	// (p2 + 1)[1] �õ� int[3]����
	printf("%08x\r\n", (p2 + 1)[1]);
	
	// p2 �õ� int[3]���͵ĵ�ַָ��
	// int[3]������һά����
	// (*p2) �õ� int���͵ĵ�ַָ��
	// (*p2)[1] �õ�int����
	// (*p2)[1] + 1�õ�int����
	printf("%08x\r\n", (*p2)[1] + 1);
	
	// p2 �õ� int[3]���͵ĵ�ַָ��
	// int[3]������һά����
	// p2 + 1 �õ� int[3]���͵ĵ�ַָ��
	// (*(p2 + 1)) �õ� int���͵�ַָ��
	// (*(p2 + 1))[1] �õ�int����
	printf("%08x\r\n", (*(p2 + 1))[1]);
	getchar();
	return 0;
}