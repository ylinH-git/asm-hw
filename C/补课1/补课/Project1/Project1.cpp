// Project1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <time.h>
/*



 补课
1.补码
2.goto循环
3.函数内存结构







计算机  x ? y = ?



位值系统


机械   二进制
0001
1111
0000

0001  mod 16
1111


3 - 1 = 3 + (mod - 1) = 2  
 
3 - 2 = 3 + （12 - 2) = 1

 (mod - 1) = 补码 = 取反 + 1



 循环
 do-while  <  while <  for

 continue;
 break;


 stack（栈）  4k

*/


int fib(int n) {
  if (n == 1 || n == 2)
    return 1;

  return fib(n-1) + fib(n - 2);
}

/*
cdecl   C调用约定
stdcall 标准调用约定
fastcall 调用约定

  保存上个函数栈底
  设置当前的栈底
  申请局部变量空间  20
  保存寄存器环境
  
  执行函数功能

  恢复寄存器环境
  栈顶设置为栈底   
  恢复栈底
  函数返回值

stack:
add:
  寄存器环境      可选
  局部变量空间    可选
  上个函数栈底
  函数返回值地址  必选
  参数1           可选
  参数2           可选
main:
  寄存器环境      可选
  局部变量空间    可选
  函数返回值地址  必选
  参数1           可选
  参数2           可选

add args:0000000a 00000014 00051023  00051023 
    0005195e 00f7fac4
main: args:00000001 01446498 01446ee0 00000001
    00052183 00f7fae4
invoke_main:57e72ef9 00051023 00051023 00c0d000
    00051fd7  00f7fb40
__scrt_common_main_seh 
      00051e6d   00f7fb48
mainCRTStartup:
      00052208   00f7fb50
syscall      75e1fa29   00f7fb60
syscall  77e07a9e 00f7fbbc
syscall  77e07a6e  00f7fbcc
*/


int Add(int n1, int n2) {
  return n1 + n2;
}

int main(int argc)  {

  Add(10, 20);
  Add(20, 30);

  //fib(2004);


  char ch1 = 3; //mod(模) 256 
  char ch2 = ~1 + 1; //(256 - 1) 1111 1111
  int n = ~0;
  ch1 = ch1 + ch2; // 255 + 256 - 1  = 254
  //255 + (256 - 1)

  printf("begin\n");
  int i;

  //do-while
//  i = 0;
//DO_WHILE: //2
//  printf("Hello World\n");
//  i++;
//  if (i < 3) 
//    goto DO_WHILE;


  //while
//  i = 0;
//WHILE:
//  if (i >= 3) 
//    goto WHILE_END;
//  printf("Hello World\n");
//  i++;
//  goto WHILE;
//WHILE_END:

//  i = 0;
//  goto WHILE2;
//WHILE1:
//  printf("Hello World\n");
//  i++;
//WHILE2:
//  if (i < 3)
//    goto WHILE1;
  
//  i = 0;
//  if (i >= 3)
//    goto WHILE_END;
//WHILE:
//  printf("Hello World\n");  
//  i++;
//  if (i < 3)
//    goto WHILE;
//WHILE_END:

//  i = 0;
//FOR_BEGIN:
//  if (i < 3)
//    goto FOR1;
//  else 
//    goto FOR_END;
//
//FOR_STEP:
//  i++;
//  goto FOR_BEGIN;
//FOR1:
//  printf("Hello World\n");  
//  goto FOR_STEP;
//FOR_END:

  clock_t begin = clock();
  for (int i = 0;
    i <  3; 
    i++) {  //步长
    printf("hello world!");  
  }

  clock_t end = clock();
  printf("time:%d\n", end- begin);

  i = 0;
  while (i < 3) {  //步长
    printf("hello world!");
    i++;
  }

  printf("end\n");

  return 0;
}