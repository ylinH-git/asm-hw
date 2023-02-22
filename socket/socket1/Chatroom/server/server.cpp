// server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include<WinSock2.h>
#include <map>
#include <string>
#define MAX_MSG_LENGTH 0x1000
using namespace std;

// 需要引入库文件
#pragma comment(lib, "Ws2_32.lib")
void InitWs2_32();
void SendErrorMesage(const SOCKET& sockServer, sockaddr_in& siClient, int errorType);
map<string, sockaddr_in> g_userList;
#define LOGIN 0
#define LOGOUT 1
#define MESSAGE 2
#define PRIVATEMESSAGE 3

struct tagLLMsgInfo
{
    int m_msgLength;
    char* m_msg;
};

struct tagPrivateMessageInfo
{
    int m_sourceNameLength;
    int m_targetNameLength;
    int m_msgLength;
    char* m_sourceName;
    char* m_targetName;
    char* m_msg;
};

struct tagMsgInfo
{
    int m_sourceNameLength;
    int m_msgLength;
    char* m_sourceName;
    char* m_msg;
};

void LoginInfoHandle(tagLLMsgInfo& loginInfo, char* szBuf, sockaddr_in& siClient, const SOCKET& sockServer)
{
    int nRet = 0;
    loginInfo.m_msgLength = *(int*)&szBuf[4];
    char* nameBuf = new char[loginInfo.m_msgLength + 1];
    memcpy_s(nameBuf, loginInfo.m_msgLength + 1, &szBuf[8], loginInfo.m_msgLength + 1);
    string sName = nameBuf;
    delete[] nameBuf;
    sockaddr_in si = {};
    si.sin_family = AF_INET;
    si.sin_port = siClient.sin_port;
    si.sin_addr.S_un = siClient.sin_addr.S_un;
    char* sendBuf = nullptr;
    if (g_userList.find(sName) == g_userList.end())
    {
        g_userList[sName] = si;
        printf(
            "来自 ip:%s port:%d 的 %s 登录成功\r\n",
            inet_ntoa(siClient.sin_addr),
            ntohs(siClient.sin_port),
            sName.c_str()
        );
        char* userListStr = nullptr;
        int userListSize = 0;
        for (auto it = g_userList.begin(); it != g_userList.end(); it++)
        {
            char* newUserListStr = new char[userListSize + it->first.size() + 1];
            if (userListStr != nullptr)
            {
                memcpy_s(newUserListStr, userListSize, userListStr, userListSize);
                memcpy_s(&newUserListStr[userListSize], it->first.size(), it->first.c_str(), it->first.size());
                delete[] userListStr;
            }
            else
            {
                memcpy_s(newUserListStr, it->first.size(), it->first.c_str(), it->first.size());
            }
            userListStr = newUserListStr;
            userListSize += it->first.size() + 1;
            newUserListStr[userListSize - 1] = '\0';
        }
        for (auto it = g_userList.begin(); it != g_userList.end(); it++)
        {
            int nError = 0;
            sockaddr_in siClient = {};
            siClient.sin_family = it->second.sin_family;
            siClient.sin_port = it->second.sin_port;
            siClient.sin_addr = it->second.sin_addr;
            size_t nSendLen = 12 + sName.size() + userListSize + 1;
            sendBuf = new char[nSendLen];
            int usersNum = g_userList.size();
            memcpy_s(sendBuf, nSendLen, (char*)&nError, sizeof(int));
            int m_type = LOGIN;
            memcpy_s(&sendBuf[4], 4, (char*)&m_type, 4);
            memcpy_s(&sendBuf[8], sizeof(int), (char*)&usersNum, sizeof(int));
            char* allNamesBuf = new char[userListSize + sName.size() + 1];
            memcpy_s(allNamesBuf, userListSize, userListStr, userListSize);
            memcpy_s(&allNamesBuf[userListSize], sName.size() + 1, sName.c_str(), sName.size() + 1);
            memcpy_s(&sendBuf[12], userListSize + sName.size() + 1, allNamesBuf, userListSize + sName.size() + 1);
            nRet = sendto(sockServer, sendBuf, nSendLen, 0, (sockaddr*)&siClient, sizeof(siClient));
            delete[] allNamesBuf;
            delete[] sendBuf;
            sendBuf = nullptr;
            if (nRet == SOCKET_ERROR)
            {
                cout << "发送失败" << endl;
            }
        }
        if (userListStr != nullptr)
        {
            delete[] userListStr;
        }
    }
    else
    {
        SendErrorMesage(sockServer, siClient, 1);
    }
}

void LogoutInfoHandle(tagLLMsgInfo& logoutInfo, char* szBuf, sockaddr_in& siClient, const SOCKET& sockServer)
{
    int nRet = 0;
    logoutInfo.m_msgLength = *(int*)&szBuf[4];
    char* nameBuf = new char[logoutInfo.m_msgLength + 1];
    memcpy_s(nameBuf, logoutInfo.m_msgLength + 1, &szBuf[8], logoutInfo.m_msgLength + 1);
    string sName = nameBuf;
    delete[] nameBuf;
    char* sendBuf = nullptr;
    auto findUser = g_userList.find(sName);
    if (findUser == g_userList.end())
    {
        SendErrorMesage(sockServer, siClient, 2);
    }
    else
    {
        for (auto it = g_userList.begin(); it != g_userList.end(); it++)
        {
            int nError = 0;
            sockaddr_in siClient = {};
            siClient.sin_family = it->second.sin_family;
            siClient.sin_port = it->second.sin_port;
            siClient.sin_addr = it->second.sin_addr;
            sendBuf = new char[8 + sName.size() + 1];
            memcpy_s(sendBuf, 4, (char*)&nError, 4);
            int m_type = LOGOUT;
            memcpy_s(&sendBuf[4], 4, (char*)&m_type, 4);
            memcpy_s(&sendBuf[8], sName.size() + 1, sName.c_str(), sName.size() + 1);
            nRet = sendto(sockServer, sendBuf, 8 + sName.size() + 1, 0, (sockaddr*)&siClient, sizeof(siClient));
            delete[] sendBuf;
            sendBuf = nullptr;
            if (nRet == SOCKET_ERROR)
            {
                cout << "发送失败" << endl;
            }
        }
        g_userList.erase(findUser);
        printf(
            "来自 ip:%s port:%d 的 %s 登出\r\n",
            inet_ntoa(siClient.sin_addr),
            ntohs(siClient.sin_port),
            sName.c_str()
        );
    }
}

void MsgInfoHandle(tagMsgInfo& msgInfo, char* szBuf, const SOCKET& sockServer)
{
    int nRet = 0;
    msgInfo.m_sourceNameLength = *(int*)&szBuf[4];
    msgInfo.m_msgLength = *(int*)&szBuf[8];
    char* nameBuf = new char[msgInfo.m_sourceNameLength + 1];
    char* msgBuf = new char[msgInfo.m_msgLength + 1];
    memcpy_s(nameBuf, msgInfo.m_sourceNameLength + 1, &szBuf[12], msgInfo.m_sourceNameLength + 1);
    memcpy_s(msgBuf, msgInfo.m_msgLength + 1, &szBuf[12 + msgInfo.m_sourceNameLength + 1], msgInfo.m_msgLength + 1);
    string sName = nameBuf;
    string sMsg = msgBuf;
    delete[] nameBuf;
    delete[] msgBuf;
    auto sourceUserFind = g_userList.find(sName);
    char* sendBuf = nullptr;
    for (auto it = g_userList.begin(); it != g_userList.end(); it++)
    {
        int nError = 0;
        string name = sourceUserFind->first;
        string sContent = sourceUserFind->first + ": " + sMsg;
        sockaddr_in siClient = {};
        siClient.sin_family = it->second.sin_family;
        siClient.sin_port = it->second.sin_port;
        siClient.sin_addr = it->second.sin_addr;
        int nMsgLen = sizeof(siClient);
        char* sendBuf = new char[8 + sContent.size() + 1];
        memcpy_s(sendBuf, 4, (char*)&nError, 4);
        int m_type = MESSAGE;
        memcpy_s(&sendBuf[4], 4, (char*)&m_type, 4);
        memcpy_s(&sendBuf[8], sContent.size(), sContent.c_str(), sContent.size());
        sendBuf[8 + sContent.size()] = '\0';
        nRet = sendto(sockServer, sendBuf, 8 + sContent.size() + 1, 0, (sockaddr*)&siClient, nMsgLen);
        delete[] sendBuf;
        sendBuf = nullptr;
        if (nRet == SOCKET_ERROR)
        {
            cout << "发送失败" << endl;
            SendErrorMesage(sockServer, siClient, 3);
        }
    }
}

void PrivateMsgInfoHandle(tagPrivateMessageInfo& msgInfo, char* szBuf, const SOCKET& sockServer)
{
    int nRet = 0;
    msgInfo.m_sourceNameLength = *(int*)&szBuf[4];
    msgInfo.m_targetNameLength = *(int*)&szBuf[8];
    msgInfo.m_msgLength = *(int*)&szBuf[12];
    char* sourceNameBuf = new char[msgInfo.m_sourceNameLength + 1];
    char* targetNameBuf = new char[msgInfo.m_targetNameLength + 1];
    char* msgBuf = new char[msgInfo.m_msgLength + 1];
    memcpy_s(sourceNameBuf, msgInfo.m_sourceNameLength + 1, &szBuf[16], msgInfo.m_sourceNameLength + 1);
    memcpy_s(targetNameBuf, msgInfo.m_targetNameLength + 1, &szBuf[16 + msgInfo.m_sourceNameLength + 1], msgInfo.m_targetNameLength + 1);
    memcpy_s(msgBuf, msgInfo.m_msgLength + 1, &szBuf[16 + msgInfo.m_sourceNameLength + msgInfo.m_targetNameLength + 2], msgInfo.m_msgLength + 1);
    string sSourceName = sourceNameBuf;
    string sTargetName = targetNameBuf;
    string sMsg = msgBuf;
    delete[] sourceNameBuf;
    delete[] targetNameBuf;
    delete[] msgBuf;
    auto sourceUserFind = g_userList.find(sSourceName);
    auto targetUserFind = g_userList.find(sTargetName);
    map<string, sockaddr_in>::iterator its[2] = { sourceUserFind, targetUserFind };
    for (int i = 0; i < 2; i++)
    {
        int nError = 0;
        string name = sourceUserFind->first;
        string sContent = sourceUserFind->first + ": " + sMsg;
        sockaddr_in siClient = {};
        siClient.sin_family = its[i]->second.sin_family;
        siClient.sin_port = its[i]->second.sin_port;
        siClient.sin_addr = its[i]->second.sin_addr;
        int nMsgLen = sizeof(siClient);
        char* sendBuf = new char[8 + sContent.size() + 1];
        memcpy_s(sendBuf, 4, (char*)&nError, 4);
        int m_type = PRIVATEMESSAGE;
        memcpy_s(&sendBuf[4], 4, (char*)&m_type, 4);
        memcpy_s(&sendBuf[8], sContent.size(), sContent.c_str(), sContent.size());
        sendBuf[8 + sContent.size()] = '\0';
        nRet = sendto(sockServer, sendBuf, 8 + sContent.size() + 1, 0, (sockaddr*)&siClient, nMsgLen);
        delete[] sendBuf;
        sendBuf = nullptr;
        if (nRet == SOCKET_ERROR)
        {
            cout << "发送失败" << endl;
            SendErrorMesage(sockServer, siClient, 3);
        }
    }
}

void SendErrorMesage(const SOCKET& sockServer, sockaddr_in& siClient, int errorType)
{
    int nRet = sendto(sockServer, (char*)&errorType, 4, 0, (sockaddr*)&siClient, sizeof(siClient));
    if (nRet == SOCKET_ERROR)
    {
        cout << "发送失败" << endl;
    }
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

    while (true)
    {
        int msgType = -1;

        char szBuf[MAX_MSG_LENGTH] = {};
        sockaddr_in siClient = {};
        int nNameLen = sizeof(siClient);
        int nRet = recvfrom(
            sockServer, 
            szBuf, 
            sizeof(szBuf),
            0,
            (sockaddr*)&siClient, 
            &nNameLen
        );

        if (nRet == 0 || nRet == SOCKET_ERROR)
        {
            cout << "接收出错" << endl;
        }
        else
        {

            msgType = *(int*)szBuf;
            tagLLMsgInfo llMsgInfo = {};
            tagMsgInfo msgInfo = {};
            tagPrivateMessageInfo privateMsgInfo = {};
            switch (msgType)
            {
            case 0:
            {
                LoginInfoHandle(llMsgInfo, szBuf, siClient, sockServer);
                break;
            }
            case 1:
                LogoutInfoHandle(llMsgInfo, szBuf, siClient, sockServer);
                break;
            case 2:
                MsgInfoHandle(msgInfo, szBuf, sockServer);
                break;
            case 3:
                PrivateMsgInfoHandle(privateMsgInfo, szBuf, sockServer);
                break;
            }
        }


        if (nRet == SOCKET_ERROR)
        {
            cout << "发送失败" << endl;
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

