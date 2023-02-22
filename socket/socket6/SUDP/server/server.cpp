// server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "CSUdp.h"
#include <thread>
#include <string>

void WorkThread(CSUdp* sudp, sockaddr_in* si);
int main()
{
    CSUdp sudp;
    sudp.CreateServer("127.0.0.1", 8888);

    while (true)
    {
        sockaddr_in siClient = sudp.Accept();
        std::thread(WorkThread,
            &sudp,
            &siClient
        ).detach();

    }
    
    system("pause");
    return 0;
}


void WorkThread(CSUdp* sudp, sockaddr_in* si)
{
    std::string fileName = "download";
    ((fileName += std::to_string(si->sin_addr.S_un.S_addr)) += std::to_string(ntohs(si->sin_port))) += ".exe";
    FILE* file = fopen(fileName.c_str(), "wb+");
    int nFileSize = 0;
    while (nFileSize == 0)
    {
        sudp->Recv((char*)&nFileSize, sizeof(nFileSize), *si);
    }
    std::cout << fileName << "开始接收" << std::endl;
    std::cout << "文件大小:" << nFileSize << "-----------------------------" << std::endl;

    int nRecvSize = 0;
    while (nRecvSize < nFileSize)
    {
        char buf[0x1000] = {};
        int nRet = sudp->Recv(buf, sizeof(buf), *si);
        if (nRet > 0)
        {
            fwrite(buf, 1, nRet, file);
            nRecvSize += nRet;
        }

    }
    fclose(file);
    sudp->Close();
    std::cout << inet_ntoa(si->sin_addr) << ":" << ntohs(si->sin_port) << "接收结束\n" << std::endl;
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
