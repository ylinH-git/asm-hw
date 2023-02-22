// server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include<WinSock2.h>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include "Proto.h"
using namespace std;

// 需要引入库文件
#pragma comment(lib, "Ws2_32.lib")
void InitWs2_32();

struct ClientInfo
{
    ClientInfo() {}
    ClientInfo(sockaddr_in si, char* szName) :
        m_si(si)
    {
        strcpy(m_szName, szName);
        m_tmLastHeart = time(NULL);
    }
    sockaddr_in m_si = {};
    char    m_szName[USERNAMELEN] = {};//用户名
    time_t  m_tmLastHeart = NULL;        //上一次心跳时间
};
map<string, ClientInfo> g_userList;

void LoginInfoHandle(const SOCKET& sockServer, sockaddr_in& siRecv, Package& pkgRecv)
{
    //1. 判断是否已在线
    string username = pkgRecv.m_szName;
    if (g_userList.find(username) != g_userList.end())
    {
        return;
    }

    //1. 通知在线的客户端，有新客户端上线
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        Package pkg(S2C_LOGIN, pkgRecv.m_szName, siRecv);
        sendto(sockServer, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&it->second.m_si, sizeof(it->second.m_si));
    }

    //2. 保存新客户端
    g_userList[pkgRecv.m_szName] = { siRecv, pkgRecv.m_szName };

    //3. 将在线客户端信息发送给新客户端
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        Package pkg(S2C_LOGIN, it->second.m_szName, it->second.m_si);
        sendto(sockServer, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&siRecv, sizeof(siRecv));
    }

    printf_s("新客户端上线  ip:%s port:%d name:%s\r\n",
        inet_ntoa(siRecv.sin_addr),
        ntohs(siRecv.sin_port),
        pkgRecv.m_szName);
}

void LogoutInfoHandle(const SOCKET& sockServer, sockaddr_in& siRecv, Package& pkgRecv)
{

    //1. 通知所有在线客户端，有客户端下线
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        Package pkg(S2C_LOGOUT, pkgRecv.m_szName, siRecv);
        sendto(sockServer, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&it->second.m_si, sizeof(it->second.m_si));
    }

    //2. 将下线客户端从链表移除
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        if (it->second.m_si == siRecv)
        {
            g_userList.erase(it);
            break;
        }
    }

    printf_s("有客户端下线  ip:%s port:%d name:%s\r\n",
        inet_ntoa(siRecv.sin_addr),
        ntohs(siRecv.sin_port),
        pkgRecv.m_szName);
}

void MsgInfoHandle(const SOCKET& sockServer, sockaddr_in& siRecv, Package& pkgRecv)
{
    // 将公聊信息转发给所有客户端
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        Package pkg(S2C_PUBMSG, pkgRecv.m_szName, siRecv, pkgRecv.m_szMsg);
        sendto(sockServer, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&it->second.m_si, sizeof(it->second.m_si));
    }

    printf_s("有客户端公聊  ip:%s port:%d name:%s msg:%s\r\n",
        inet_ntoa(siRecv.sin_addr),
        ntohs(siRecv.sin_port),
        pkgRecv.m_szName,
        pkgRecv.m_szMsg);
}

void PrivateMsgInfoHandle(const SOCKET& sockServer, sockaddr_in& siRecv, Package& pkgRecv)
{
    Package pkg(S2C_PRIMSG, pkgRecv.m_szName, siRecv, pkgRecv.m_siDst, pkgRecv.m_szMsg);

    //发回
    sendto(sockServer, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&siRecv, sizeof(siRecv));

    //转发
    sendto(sockServer, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&pkgRecv.m_siDst, sizeof(pkgRecv.m_siDst));


    printf_s("有客户端私聊  ip:%s port:%d name:%s msg:%s to ip:%s port:%d \r\n",
        inet_ntoa(siRecv.sin_addr),
        ntohs(siRecv.sin_port),
        pkgRecv.m_szName,
        pkgRecv.m_szMsg,
        inet_ntoa(pkgRecv.m_siDst.sin_addr),
        ntohs(pkgRecv.m_siDst.sin_port)
    );
}

int main()
{
    InitWs2_32();

    SOCKET sockServer = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_UDP
    );

    if (sockServer == INVALID_SOCKET)
    {
        cout << "socket 创建失败" << endl;
        return 0;
    }

    sockaddr_in si = {};
    si.sin_family = AF_INET;
    si.sin_port = htons(8888);

    si.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(sockServer, (sockaddr*)&si, sizeof(si)) == SOCKET_ERROR)
    {
        cout << "端口绑定失败" << endl;
        return 0;
    }
    else
    {
        cout << "连接成功，等待收发消息……" << endl;
    }

    recursive_mutex mtxForLst;
    //启动心跳检查线程
    thread([&sockServer, &mtxForLst]() {
        while (true)
        {
            mtxForLst.lock();
            for (auto it = g_userList.begin(); it != g_userList.end(); it++)
            {
                if (time(NULL) - it->second.m_tmLastHeart > 2) //超时2秒，认为下线
                {
                    //1.移除
                    ClientInfo ciToLogout = it->second;
                    g_userList.erase(it);

                    //2. 通知其他在线客户端，有客户端下线
                    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
                    {
                        Package pkg(S2C_LOGOUT, ciToLogout.m_szName, ciToLogout.m_si);
                        sendto(sockServer, (char*)&pkg, sizeof(pkg), 0, (sockaddr*)&it->second.m_si, sizeof(it->second.m_si));
                    }

                    printf("有客户端下线  ip:%s port:%d name:%s\r\n",
                        inet_ntoa(ciToLogout.m_si.sin_addr),
                        ntohs(ciToLogout.m_si.sin_port),
                        ciToLogout.m_szName);
                    break;
                }
            }
            mtxForLst.unlock();
            //切出线程
            this_thread::yield();
        }
        }).detach();

    while (true)
    {
        //接受包
        Package pkgRecv = {};
        sockaddr_in siRecv = {};
        int nSiSize = sizeof(siRecv);
        int nRet = recvfrom(sockServer, (char*)&pkgRecv, sizeof(pkgRecv), 0, (sockaddr*)&siRecv, &nSiSize);
        if (nRet == 0 || nRet == SOCKET_ERROR)
        {
            continue;
        }

        //处理包
        lock_guard<recursive_mutex> lck(mtxForLst);
        switch (pkgRecv.m_cmd)
        {
        case C2S_LOGIN:
        {
            LoginInfoHandle(sockServer,siRecv, pkgRecv);
            break;
        }
        case C2S_LOGOUT:
        {
            LogoutInfoHandle(sockServer, siRecv, pkgRecv);
            break;
        }
        case C2S_PUBMSG:
        {
            MsgInfoHandle(sockServer, siRecv, pkgRecv);
            break;
        }
        case C2S_PRIMSG:
        {
            PrivateMsgInfoHandle(sockServer, siRecv, pkgRecv);
            break;
        }
        case C2S_HEART:
        {

            for (auto it = g_userList.begin(); it != g_userList.end(); it++)
            {
                if (it->second.m_si == siRecv)
                {
                    it->second.m_tmLastHeart = time(NULL);
                }
            }
        }
        default:
            break;
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

