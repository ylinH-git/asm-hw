#pragma once
#include <WinSock2.h>

#define USERNAMELEN 32
#define MSGLEN      512

//包命令
enum PackCmd //package command
{
  C2S_LOGIN, //c->s client server 客户端发送给服务器的包， 登录
  S2C_LOGIN, //s->c

  C2S_LOGOUT, //下线
  S2C_LOGOUT, //下线

  C2S_PUBMSG, //公聊
  S2C_PUBMSG, //公聊

  C2S_PRIMSG, //私聊
  S2C_PRIMSG, //私聊

  C2S_HEART   //心跳
};

//数据包

//#pragma pack(push)
//#pragma pack(1)
//#pragma pack(pop)
struct Package
{
  Package() {}
  Package(PackCmd cmd, char* szUserName) :
    m_cmd(cmd)
  {
    strcpy(m_szName, szUserName);
  }
  Package(PackCmd cmd, char* szUserName, char* szMsg) :
    m_cmd(cmd)
  {
    strcpy(m_szName, szUserName);
    strcpy(m_szMsg, szMsg);
  }
  Package(PackCmd cmd, char* szUserName, sockaddr_in siFrom) :
    m_cmd(cmd),
    m_siFrom(siFrom)
  {
    strcpy(m_szName, szUserName);
  }
  Package(PackCmd cmd, char* szUserName, sockaddr_in siFrom, char* szMsg) :
    Package(cmd, szUserName, siFrom)
  {
    strcpy(m_szMsg, szMsg);
  }
  Package(PackCmd cmd, char* szUserName, sockaddr_in siFrom, sockaddr_in siDst, char* szMsg) :
    Package(cmd, szUserName, siFrom, szMsg)
  {
    m_siDst = siDst;
  }

  PackCmd m_cmd;                //包命令
  sockaddr_in m_siFrom;         //由谁发送
  sockaddr_in m_siDst;          //由谁接受
  char    m_szName[USERNAMELEN];//用户名
  char    m_szMsg[MSGLEN];      //聊天信息
};

inline bool operator==(const sockaddr_in& siL, const sockaddr_in& siR)
{
  return (siL.sin_addr.S_un.S_addr == siR.sin_addr.S_un.S_addr) && (siL.sin_port == siR.sin_port);
}
