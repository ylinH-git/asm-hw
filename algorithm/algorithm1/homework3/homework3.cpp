// homework3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

int main()
{
    int i, j, s = 0, m =5, n = 5;
    for (i = 1;i <= m;i++)
    {
        for (j = 1; j <= n; j++)
            s++;
        printf("%d", s);
    }
    std::cout << "Hello World!\n";
}

