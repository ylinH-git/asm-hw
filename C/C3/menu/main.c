#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main() {
  char key;
  printf(
    "���˵�\n"
    "================================\n"
    "1. ���빦��\n"
  	"2. ��ѧ�Ų���\n"
 		"3. ��ӡ���\n"
  	"0. �˳�\n"
    "================================\n"
    "��ѡ�� (0~3):\n"
  );
  scanf("%d", &key);
  switch(key) {
    case 1: 
      printf("������\n");
      break;
    case 2: 
      printf("���������ѧ��ѧ��\n");
      break;
    case 3: 
      printf("�������\n");
      break;
    case 0: 
      printf("ллʹ��\n");
      break;
    default:  
      printf("�������\n");
      break;
  }
  system("pause");
  return 0;
}

