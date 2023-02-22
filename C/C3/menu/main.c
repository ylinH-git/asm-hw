#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main() {
  char key;
  printf(
    "主菜单\n"
    "================================\n"
    "1. 输入功能\n"
  	"2. 按学号查找\n"
 		"3. 打印输出\n"
  	"0. 退出\n"
    "================================\n"
    "请选择 (0~3):\n"
  );
  scanf("%d", &key);
  switch(key) {
    case 1: 
      printf("请输入\n");
      break;
    case 2: 
      printf("请输入查找学生学号\n");
      break;
    case 3: 
      printf("正在输出\n");
      break;
    case 0: 
      printf("谢谢使用\n");
      break;
    default:  
      printf("输入错误\n");
      break;
  }
  system("pause");
  return 0;
}

