// think.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

int main()
{
    int nVal1 = 8;
    int nVal2 = 9;

    // p1和p2都是指向的值是常量的指针，可以被赋予成int指针
    const int* p1 = &nVal1;
    int const* p2 = &nVal2;

    // p3\p4\p5是指向值和指针本身都是常量的指针，所以可以接受从变量到常量的转换
    const int* const p3 = &nVal1;
    const int* const p4 = p1;
    const int* const p5 = p2;

    // p1是const int*类型，不能转换为int*类型
    int* p6 = p1;
    // p3是指针和指针指向的值都是常量，更不能转换为int*类型
    int* p7 = p3;
    // p3指向的值是个常量，p8定义指向的值是常量，
    // p8能接受指针常量的转换，所以p3可以给p8
    const int* p8 = p3;
    // p9定义指向的值是个一个变量，p3定义指向的值是一个常量，所以不能转换，否则p3能被改变
    int* const p9 = p3;
    // 同上
    int* const p10 = p1;
}
