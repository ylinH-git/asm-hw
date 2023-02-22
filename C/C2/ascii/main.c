#include <stdio.h>
#include <stdlib.h>
#define ASCII 128
int main() {
	int i;
	printf("   ascii dec hex\n");
	for(i = 0; i <= ASCII; i++) {
		switch(i) {
			case 7:
				printf("    铃声 %03d %03x\n", i, i);
				break;
			case 8:
				printf("    退格 %03d %03x\n", i, i);
				break;
			case 9:
				printf("水平制表 %03d %03x\n", i, i);
				break;
			case 10:
				printf("    换行 %03d %03x\n", i, i);
				break;
			case 11:
				printf("竖直制表 %03d %03x\n", i, i);
				break;
			case 12:
				printf("    换页 %03d %03x\n", i, i);
				break;
			case 13:
				printf("    回车 %03d %03x\n", i, i);
				break;
			case 14:
				printf("    移出 %03d %03x\n", i, i);
				break;
			case 15:
				printf("    移入 %03d %03x\n", i, i);
				break;
			default:
				printf("%8c %03d %03x\n", i, i, i);
				break;
		}		
	}
	system("pause");
	return 0;
}
