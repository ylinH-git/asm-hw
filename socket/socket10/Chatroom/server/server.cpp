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


void InitWs2_32();


int PostAccept(SOCKET sockListen, HANDLE hIocp);

struct ClientInfo
{
    ClientInfo() {}
    ClientInfo(SOCKET sock, sockaddr_in si, char* szName) :
        m_sock(sock),
        m_si(si)
    {
        strcpy(m_szName, szName);
    }
    SOCKET  m_sock = INVALID_SOCKET;
    sockaddr_in m_si = {};
    char m_szName[USERNAMELEN] = {};//用户名
};
map<string, ClientInfo> g_userList;

void LoginInfoHandle(const SOCKET& sock, const sockaddr_in& si, const shared_ptr<ProtoHeader>& ptrProto)
{

    //1. 判断是否已在线
    C2SLogin* pLoginPkg = (C2SLogin*)ptrProto.get();
    string username = pLoginPkg->m_szName;
    if (g_userList.find(username) != g_userList.end())
    {
        return;
    }

    //2. 通知在线的客户端，有新客户端上线
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        S2CLogin pkg(PackCmd::S2C_LOGIN, si, pLoginPkg->m_szName);
        CTcp(it->second.m_sock).WritePackage(&pkg);
    }

    //3. 保存新客户端
    g_userList[username] = { sock, si, pLoginPkg->m_szName };

    //4. 将在线客户端信息发送给新客户端
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        S2CLogin pkg(PackCmd::S2C_LOGIN, it->second.m_si, it->second.m_szName);
        CTcp(sock).WritePackage(&pkg);
    }

    printf("新客户端上线  ip:%s port:%d name:%s\r\n",
        inet_ntoa(si.sin_addr),
        ntohs(si.sin_port),
        pLoginPkg->m_szName);
}

void LogoutInfoHandle(const SOCKET& sock, const sockaddr_in& si, const shared_ptr<ProtoHeader>& ptrProto)
{

    C2SLogout* pLogout = (C2SLogout*)ptrProto.get();

    //1. 通知所有在线客户端，有客户端下线
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        S2CLogout pkg(PackCmd::S2C_LOGOUT, si, pLogout->m_szName);
        CTcp(it->second.m_sock).WritePackage(&pkg);
    }

    //2. 将下线客户端从链表移除
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        if (it->second.m_si == si)
        {
            g_userList.erase(it);
            break;
        }
    }

    printf("有客户端下线  ip:%s port:%d name:%s\r\n",
        inet_ntoa(si.sin_addr),
        ntohs(si.sin_port),
        pLogout->m_szName);
}

void MsgInfoHandle(const SOCKET& sock, const sockaddr_in& si, const shared_ptr<ProtoHeader>& ptrProto)
{
    C2SPubMsg* pPubmsg = (C2SPubMsg*)ptrProto.get();

    // 将公聊信息转发给所有客户端
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        S2CPubMsg pkg(PackCmd::S2C_PUBMSG, pPubmsg->m_szName, pPubmsg->m_szMsg);
        CTcp(it->second.m_sock).WritePackage(&pkg);
    }
    printf("有客户端公聊  ip:%s port:%d name:%s msg:%s\r\n",
        inet_ntoa(si.sin_addr),
        ntohs(si.sin_port),
        pPubmsg->m_szName,
        pPubmsg->m_szMsg);
}

void PrivateMsgInfoHandle(const SOCKET& sock, const sockaddr_in& si, const shared_ptr<ProtoHeader>& ptrProto)
{

    C2SPriMsg* pPriMsg = (C2SPriMsg*)ptrProto.get();
    S2CPriMsg pkg(PackCmd::S2C_PRIMSG, pPriMsg->m_siDst, pPriMsg->m_szName, pPriMsg->m_szMsg);

    //发回
    CTcp(sock).WritePackage(&pkg);

    //转发
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        if (it->second.m_si == pPriMsg->m_siDst)
        {
            CTcp(it->second.m_sock).WritePackage(&pkg);
            break;
        }
    }

    printf("有客户端私聊  ip:%s port:%d name:%s msg:%s to ip:%s port:%d \r\n",
        inet_ntoa(si.sin_addr),
        ntohs(si.sin_port),
        pPriMsg->m_szName,
        pPriMsg->m_szMsg,
        inet_ntoa(pPriMsg->m_siDst.sin_addr),
        ntohs(pPriMsg->m_siDst.sin_port)
    );
}


void WorkThread(SOCKET sock, sockaddr_in si, recursive_mutex& mtx)
{
    CTcp tcp(sock);

    while (true)
    {
        //收包
        
    }
}


int main()
{
    InitWs2_32();

    //创建socket
    SOCKET sockListen = socket(AF_INET/*ipv4*/, SOCK_STREAM/*数据流*/, IPPROTO_TCP);
    if (sockListen == INVALID_SOCKET)
    {
        cout << "socket 创建失败" << endl;
        return 0;
    }

    //绑定端口
    sockaddr_in si = {};
    si.sin_family = AF_INET;
    si.sin_port = htons(8888);
    si.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(sockListen, (sockaddr*)&si, sizeof(si)) == SOCKET_ERROR)
    {
        cout << "端口绑定失败" << endl;
        return 0;
    }

    //3. 监听
    if (listen(sockListen, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "监听失败" << endl;
        return 0;
    }
    
    // 1. 创建IOCP
    HANDLE hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
    if (hIocp == NULL)
    {
        cout << "创建iocp失败" << endl;
        return 0;
    }

    // 2. 注册socket
    HANDLE hRet = CreateIoCompletionPort((HANDLE)sockListen, hIocp, NULL, 0);
    if (hRet == NULL)
    {
        cout << "注册socket失败" << endl;
        return 0;
    }

    // 3.投递接收连接的请求
    PostAccept(sockListen, hIocp);

    while (true)
    {
        DWORD dwNumberOfBytesTransferred = 0;
        ULONG_PTR nCompletionKey = 0;
        CUSTOMOVERLAPPED* pOv = NULL;
        BOOL bRet = GetQueuedCompletionStatus(hIocp, &dwNumberOfBytesTransferred, &nCompletionKey, (LPOVERLAPPED*)&pOv, INFINITE);
        if (!bRet)
        {
            continue;
        }
        switch (pOv->m_of)
        {
        case OVERLAPPED_FLAG::OF_ACCEPT:
        {
            PostAccept(sockListen, hIocp);
            CUSTOMOVERLAPPED* pRecvOv = new CUSTOMOVERLAPPED(OVERLAPPED_FLAG::OF_RECV, pOv->m_sock);
            int nRet = WSARecv(pRecvOv->m_sock, &pRecvOv->m_wbuf, 1, 0, &pRecvOv->m_dwFlag, pRecvOv, NULL);
            break;
        }
        case OVERLAPPED_FLAG::OF_RECV:
        {
            CTcp tcp(pOv->m_sock);
            shared_ptr<ProtoHeader> ptrProto = tcp.ReadPackage();
            if (ptrProto == nullptr)
            {
                //掉线
                continue;
            }
            sockaddr_in* pLocal = nullptr;
            sockaddr_in* pClient = nullptr;
            int nLocalSize = 0;
            int nClientSize = 0;
            GetAcceptExSockaddrs(
                pOv->m_bufAccept,
                0, sizeof(sockaddr_in) + 16,
                sizeof(sockaddr_in) + 16,
                (sockaddr**)&pLocal, &nLocalSize,
                (sockaddr**)&pClient, &nClientSize
            );
            switch (ptrProto->m_cmd)
            {
            case PackCmd::C2S_LOGIN:
            {
                LoginInfoHandle(pOv->m_sock, *pClient, ptrProto);
                break;
            }
            case PackCmd::C2S_LOGOUT:
            {
                LogoutInfoHandle(pOv->m_sock, *pClient, ptrProto);
                break;
            }
            case PackCmd::C2S_PUBMSG:
            {
                MsgInfoHandle(pOv->m_sock, *pClient, ptrProto);
                break;
            }
            case PackCmd::C2S_PRIMSG:
            {
                PrivateMsgInfoHandle(pOv->m_sock, *pClient, ptrProto);
                break;
            }
            default:
                break;
            }
            break;
        }
        case OVERLAPPED_FLAG::OF_SEND:
            break;
        default:
            break;
        }



        if (pOv != nullptr)
        {
            delete pOv;
        }
    }

#if 0
    map<SOCKET, sockaddr_in> lstSocks;
    lstSocks[sockAccept] = si;
    //客户端信息
    while (true)
    {
        fd_set fs = {};
        for (auto& sock : lstSocks)
        {
            FD_SET(sock.first, &fs);
        }

        TIMEVAL tm = { 1, 0 };
        int nRet = select(fs.fd_count, &fs, NULL, NULL, &tm);
        if (nRet == 0 || nRet == SOCKET_ERROR)
        {
            continue;;
        }

        for (size_t i = 0; i < nRet; ++i)
        {
            if (fs.fd_array[i] == sockAccept)
            {
                //4. 接受连接
                sockaddr_in siClient = {};
                int nSockaddrSize = sizeof(siClient);
                SOCKET sockClient = accept(sockAccept, (sockaddr*)&siClient, &nSockaddrSize);
                if (sockClient == INVALID_SOCKET)
                {
                    cout << "连接失败" << endl;
                }

                cout << "客户端建立连接 ip:" << inet_ntoa(siClient.sin_addr)
                    << " port: " << ntohs(siClient.sin_port)
                    << endl;

                lstSocks[sockClient] = siClient;
            }
            else
            {
                //客户端信息
                auto sockClient = fs.fd_array[i];
                auto& siClient = lstSocks[sockClient];
                CTcp tcp(sockClient);

                shared_ptr<ProtoHeader> ptrProto = tcp.ReadPackage();
                if (ptrProto == nullptr)
                {
                    //掉线
                    continue;
                }
                //mtxForLst.lock();
                //处理包
                switch (ptrProto->m_cmd)
                {
                case PackCmd::C2S_LOGIN:
                {
                    LoginInfoHandle(sockClient, siClient, ptrProto);
                    break;
                }
                case PackCmd::C2S_LOGOUT:
                {
                    LogoutInfoHandle(sockClient, siClient, ptrProto);
                    break;
                }
                case PackCmd::C2S_PUBMSG:
                {
                    MsgInfoHandle(sockClient, siClient, ptrProto);
                    break;
                }
                case PackCmd::C2S_PRIMSG:
                {
                    PrivateMsgInfoHandle(sockClient, siClient, ptrProto);
                    break;
                }
                default:
                    break;
                }
                //mtxForLst.unlock();
            }
        }

    }
#endif // 0
    closesocket(sockListen);
}

int PostAccept(SOCKET sockListen, HANDLE hIocp)
{
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    HANDLE hRet = CreateIoCompletionPort((HANDLE)sockClient, hIocp, NULL, 0);
    if (hRet == NULL)
    {
        cout << "注册socket失败" << endl;
        return 0;
    }

    CUSTOMOVERLAPPED* pOv = new CUSTOMOVERLAPPED(OVERLAPPED_FLAG::OF_ACCEPT, sockClient);
    char bufAccept[(sizeof(sockaddr_in) + 16) * 2] = {};

    if (sockClient == INVALID_SOCKET)
    {
        cout << "socket 创建失败" << endl;
        return 0;
    }
    BOOL bRet = AcceptEx(
        sockListen,
        sockClient,
        pOv->m_bufAccept,
        0,
        sizeof(sockaddr_in) + 16,
        sizeof(sockaddr_in) + 16,
        NULL,
        pOv
    );

    if (!bRet && GetLastError() != ERROR_IO_PENDING)
    {
        cout << "投递请求失败" << endl;
        return 0;
    }
    return bRet;
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

