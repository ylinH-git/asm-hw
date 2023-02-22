// server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
// Windows.h内有一个旧的sock库，所以下面这种写法会报错
// 为了避免报错有下面几种方法
// 1. 调转顺序，先包括WinSock2.h
// 2. 调转顺序不报错的原因是WinSock2.h打开了WIN32_LEAN_AND_MEAN开关
// 我们也可以手动把这个接入到预处理器、或者手动#define
#include <Windows.h>
#include <WinSock2.h>
#include "Proto.h"
using namespace std;

// 需要引入库文件
#pragma comment(lib, "Ws2_32.lib")
void InitWs2_32();

int main()
{
    InitWs2_32();
    SOCKET sockServer = socket(
        AF_INET/*ipv4*/,
        SOCK_DGRAM/*数据报*/,
        IPPROTO_UDP
    );
    if (sockServer == INVALID_SOCKET)
    {
        cout << "socket 创建失败" << endl;
        return 0;
    }

    sockaddr_in si = {};
    si.sin_family = AF_INET;
    si.sin_port = htons(9527);
    si.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(sockServer, (sockaddr*)&si, sizeof(si)) == SOCKET_ERROR)
    {
        cout << "端口绑定失败" << endl;
        return 0;
    }

    while (true)
    {
        //收发数据
        char szBuf[0x1000] = {};
        sockaddr_in siClient = {};
        int nFormLen = sizeof(siClient);
        int nRet = recvfrom(sockServer, szBuf, sizeof(szBuf),
            0,
            (sockaddr*)&siClient, &nFormLen);
        if (nRet == 0 || nRet == SOCKET_ERROR)
        {
            cout << "接受出错" << endl;
            continue;
        }
        Package* recvPkg = (Package*) szBuf;
        if (recvPkg->m_flag == PkgFlags::SYNACK)
        {
            char rBuf[512] = {};
            memcpy_s(rBuf, recvPkg->m_segLen, recvPkg->m_seg, recvPkg->m_segLen);
            printf("收到来自 ip:%s port:%d 的数据: %s , 当前序号：%d 下一个序号：%d\r\n",
                inet_ntoa(siClient.sin_addr),
                ntohs(siClient.sin_port),
                rBuf,
                recvPkg->m_seq,
                recvPkg->m_seq + recvPkg->m_segLen
            );
            Package pkg(si, siClient, PkgFlags::ACK, recvPkg->m_seq + recvPkg->m_segLen, 0, nullptr);
            int nRet = sendto(sockServer,
                (char*)&pkg,
                sizeof(PkgHeader) + pkg.m_segLen,
                0,
                (sockaddr*)&pkg.m_addrDst,
                sizeof(pkg.m_addrDst)
            );
            if (nRet == 0 || nRet == SOCKET_ERROR)
            {
                cout << "发送失败" << endl;
            }
        }
    }
    closesocket(sockServer);
}


void InitWs2_32()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        return;
    }

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        WSACleanup();
        return;
    }

    /* The WinSock DLL is acceptable. Proceed. */
}
