#include <stdio.h>
#include <stdlib.h>
#define ASCII 128
int main() {
	int i;
	printf("   ascii dec hex\n");
	for(i = 0; i <= ASCII; i++) {
		switch(i) {
			case 7:
				printf("    ���� %03d %03x\n", i, i);
				break;
			case 8:
				printf("    �˸� %03d %03x\n", i, i);
				break;
			case 9:
				printf("ˮƽ�Ʊ� %03d %03x\n", i, i);
				break;
			case 10:
				printf("    ���� %03d %03x\n", i, i);
				break;
			case 11:
				printf("��ֱ�Ʊ� %03d %03x\n", i, i);
				break;
			case 12:
				printf("    ��ҳ %03d %03x\n", i, i);
				break;
			case 13:
				printf("    �س� %03d %03x\n", i, i);
				break;
			case 14:
				printf("    �Ƴ� %03d %03x\n", i, i);
				break;
			case 15:
				printf("    ���� %03d %03x\n", i, i);
				break;
			default:
				printf("%8c %03d %03x\n", i, i, i);
				break;
		}		
	}
	system("pause");
	return 0;
}
