// client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CSUdp.h"
int main()
{
    CSUdp sudp;
    sudp.CreateClient("127.0.0.1", 8888);

    if (sudp.Connect())
    {
        sockaddr_in* pSi = sudp.GetSrc();
        FILE* file = fopen("e:\\download.exe", "rb+");
        fseek(file, 0, FILE_END);
        int nFileSize = ftell(file);
        fseek(file, 0, FILE_BEGIN);

        std::cout << "文件大小:" << nFileSize << std::endl;
        sudp.Send((char*)&nFileSize, sizeof(nFileSize));

        int nSendedSize = 0;
        while (nSendedSize < nFileSize)
        {
            char buf[0x1000] = {};
            int nReadSize = fread(buf, 1, sizeof(buf), file);
            sudp.Send(buf, nReadSize);

            nSendedSize += nReadSize;
        }
        std::cout << inet_ntoa(pSi->sin_addr) << ":" << ntohs(pSi->sin_port) << "传输结束\n" << std::endl;
        fclose(file);
    }

    system("pause");
    sudp.Close();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
