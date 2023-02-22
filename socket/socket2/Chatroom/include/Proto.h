#pragma once
#include <WinSock2.h>

#define USERNAMELEN 32
#define MSGLEN      512

//������
enum PackCmd //package command
{
  C2S_LOGIN, //c->s client server �ͻ��˷��͸��������İ��� ��¼
  S2C_LOGIN, //s->c

  C2S_LOGOUT, //����
  S2C_LOGOUT, //����

  C2S_PUBMSG, //����
  S2C_PUBMSG, //����

  C2S_PRIMSG, //˽��
  S2C_PRIMSG, //˽��

  C2S_HEART   //����
};

//���ݰ�

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

  PackCmd m_cmd;                //������
  sockaddr_in m_siFrom;         //��˭����
  sockaddr_in m_siDst;          //��˭����
  char    m_szName[USERNAMELEN];//�û���
  char    m_szMsg[MSGLEN];      //������Ϣ
};

inline bool operator==(const sockaddr_in& siL, const sockaddr_in& siR)
{
  return (siL.sin_addr.S_un.S_addr == siR.sin_addr.S_un.S_addr) && (siL.sin_port == siR.sin_port);
}
