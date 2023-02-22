// client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include "Proto.h"

using namespace std;

// 需要引入库文件
#pragma comment(lib, "Ws2_32.lib")
void InitWs2_32();

int Connect(SOCKET sock, sockaddr_in& server)
{
    Package pkg({}, server, PkgFlags::SYNACK, 100, 0, nullptr);
    return WritePackage(sock, (PkgHeader*)&pkg);
}

int main()
{
    InitWs2_32();
    //创建socket
    SOCKET sockServer = socket(AF_INET/*ipv4*/, SOCK_DGRAM/*数据报*/, IPPROTO_UDP);
    if (sockServer == INVALID_SOCKET)
    {
        cout << "socket 创建失败" << endl;
        return 0;
    }

    //服务器ip和端口
    sockaddr_in siServer = {};
    siServer.sin_family = AF_INET;
    siServer.sin_port = htons(9527);
    siServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if (Connect(sockServer, siServer))
    {
        cout << "连接失败" << endl;
        return 0;
    }

    char szBuf[MAXBYTE] = {};
    cin.getline(szBuf, sizeof(szBuf));
    Package pkg({}, siServer, PkgFlags::SYNACK, 100, strlen(szBuf), szBuf);
    WritePackage(sockServer, (PkgHeader *)&pkg);
    closesocket(sockServer);
}

void InitWs2_32()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    // 微软用来初始化"Ws2_32.lib"的api，这个是微软特有的
    // linux不需要，虽然上面的创建socket的代码其实是一样的
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

