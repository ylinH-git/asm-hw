#pragma once
#include <WinSock2.h>
#include <memory>
#define USERNAMELEN 32
#define MSGLEN      512

using namespace std;
//包命令
enum class PackCmd //package command
{
	C2S_LOGIN, //c->s client server 客户端发送给服务器的包， 登录
	S2C_LOGIN, //s->c

	C2S_LOGOUT, //下线
	S2C_LOGOUT, //下线

	C2S_PUBMSG, //公聊
	S2C_PUBMSG, //公聊

	C2S_PRIMSG, //私聊
	S2C_PRIMSG, //私聊
};

struct ProtoHeader
{
	ProtoHeader() {}
	ProtoHeader(PackCmd cmd, int nDataLen):
		m_cmd(cmd), m_nDataLen(nDataLen)
	{}
	PackCmd m_cmd = PackCmd::C2S_LOGIN;
	int m_nDataLen = 0;
};

struct C2SLogin : public ProtoHeader
{
	C2SLogin(PackCmd cmd, char* szName) :
		ProtoHeader(cmd, sizeof(C2SLogin) - sizeof(ProtoHeader))
	{
		strcpy(m_szName, szName);
	}
	char m_szName[USERNAMELEN];
};
using C2SLogout = C2SLogin;


struct S2CLogin : public ProtoHeader
{
	S2CLogin(PackCmd cmd, sockaddr_in siLogin, char* szName) :
		ProtoHeader(cmd, sizeof(S2CLogin) - sizeof(ProtoHeader)),
		m_siLogin(siLogin)
	{
		strcpy(m_szName, szName);
	}
	sockaddr_in m_siLogin;
	char m_szName[USERNAMELEN];
};
using S2CLogout = S2CLogin;

struct C2SPubMsg :public ProtoHeader
{
	C2SPubMsg(PackCmd cmd, char* szName, char* szMsg) :
		ProtoHeader(cmd, sizeof(C2SPubMsg) - sizeof(ProtoHeader))
	{
		strcpy(m_szName, szName);
		strcpy(m_szMsg, szMsg);
	}
	char m_szName[USERNAMELEN];
	char m_szMsg[MSGLEN];
};
using S2CPubMsg = C2SPubMsg;

struct C2SPriMsg :public ProtoHeader
{
	C2SPriMsg(PackCmd cmd, sockaddr_in siDst, char* szName, char* szMsg) :
		ProtoHeader(cmd, sizeof(C2SPriMsg) - sizeof(ProtoHeader)),
		m_siDst(siDst)
	{
		strcpy(m_szName, szName);
		strcpy(m_szMsg, szMsg);
	}
	char m_szName[USERNAMELEN];
	char m_szMsg[MSGLEN];
	sockaddr_in m_siDst;
};
using S2CPriMsg = C2SPriMsg;

inline bool operator==(const sockaddr_in& siL, const sockaddr_in& siR)
{
	return (siL.sin_addr.S_un.S_addr == siR.sin_addr.S_un.S_addr) && (siL.sin_port == siR.sin_port);
}


class CTcp
{
public:
	CTcp(SOCKET sock) :m_sock(sock) {}
	shared_ptr<ProtoHeader> ReadPackage()
	{
		// 先读包头
		ProtoHeader hdr = {};
		int nRet = ReadBytes((char*)&hdr, sizeof(hdr));
		if (nRet == 0)
		{
			return 0;
		}
		// 再读数据
		shared_ptr<ProtoHeader> ptrProto((ProtoHeader*)new char[sizeof(hdr) + hdr.m_nDataLen]);
		memcpy(ptrProto.get(), &hdr, sizeof(hdr));
		nRet = ReadBytes((char*)ptrProto.get() + sizeof(hdr), hdr.m_nDataLen);
		if (nRet == 0)
		{
			return nullptr;
		}
		// 返回包
		return ptrProto;
	}
	int WritePackage(shared_ptr<ProtoHeader> ptrProto)
	{
		int nRet = send(m_sock,
			(char*)ptrProto.get(),
			sizeof(ProtoHeader) + ptrProto->m_nDataLen,
			0);
		return nRet == SOCKET_ERROR ? 0 : nRet;
	}
	int WritePackage(ProtoHeader* proto)
	{
		int nRet = send(m_sock,
			(char*)proto,
			sizeof(ProtoHeader) + proto->m_nDataLen,
			0);
		return nRet == SOCKET_ERROR ? 0 : nRet;
	}
private:
	int ReadBytes(char* buf, int nLen)
	{
		int nBytesRecv = 0;
		while (nBytesRecv < nLen)
		{
			int nRet = recv(m_sock, buf + nBytesRecv, nLen - nBytesRecv, 0);
			if ((nRet == 0) ||
				(nRet == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
			{
				return 0;
			}
			else if (nRet == SOCKET_ERROR)
			{
				continue;
			}
			nBytesRecv += nRet;
		}

		return nBytesRecv;
	}
private:
	SOCKET m_sock;
};