#include <stdio.h>
#include <stdlib.h>

#define MAX 100

/* 9332621544394415268169923885626670049071596826
   4381621468592963895217599993229915608941463976
   1565182862536979208272237582511852109168640000
   00000000000000000000 */
int main() {
	int arr[256] = {1}, carry = 0, cnt = 1, i, j, num;

	for(i = 0; i < MAX; i++) {
		for(j = 0; j < cnt; j++) {
			num = arr[j] * (i + 1) + carry;
			arr[j] = num % 10;
			carry = num / 10;
		}
		// 这里要注意carry在!100可能为3位数
		while (carry)
		{
			arr[cnt++] = carry % 10;
			carry /= 10;
		}
	}

	for(i = cnt - 1; i >= 0; i--){
		printf("%d", arr[i]);
	}
	system("pause");
	return 0;
}
