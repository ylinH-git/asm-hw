#pragma once
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <string>

#include <WinSock2.h>
#include <Windows.h>

#define MTU 60000
#define HDRSIZE 11
#define MSS (MTU-HDRSIZE) // 最大数据长度
#define RESENDTIMEOUT 20
#define WINDOWSIZE 100

#if 0
struct sockaddrcomp {
	bool operator() (const sockaddr_in& lhs, const sockaddr_in& rhs) const
	{
		if (lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr)
		{
			return lhs.sin_port < rhs.sin_port;
		}
		else
		{
			return lhs.sin_addr.S_un.S_addr < rhs.sin_addr.S_un.S_addr;
		}

	}
};
#endif // 0


enum class PackFlag
{
	PF_PSH = 1,
	PF_ACK = 2,
	PF_SYN = 4,
	PF_FIN = 8,
};

#pragma comment(lib, "Ws2_32.lib")
#pragma pack(push)
#pragma pack(1)

struct CPackage
{
	CPackage() {}
	CPackage(char nFlag, int nSeq) :
		m_nFlag(nFlag),
		m_nSeq(nSeq),
		m_nChek(0),
		m_nLen(0)
	{
	}
	CPackage(char nFlag, int nSeq, const char* buf, int nLen) :
		m_nFlag(nFlag),
		m_nSeq(nSeq),
		m_nLen(nLen)
	{
		RtlCopyMemory(m_buf, buf, nLen);
		m_nChek = Crc32(buf, nLen);
	}
	int Crc32(const char* pData, int nLen);
	bool IsCheckOK();
	char m_nFlag;
	int m_nSeq;
	int m_nChek;
	short m_nLen;
	char m_buf[MSS];
};

struct PackageInfo
{
	PackageInfo() {}
	PackageInfo(bool bIsCheckTimeOut, time_t nLastSend, CPackage pck):
		m_bIsCheckTimeOut(bIsCheckTimeOut), m_nLastSend(nLastSend), m_pck(pck)
	{

	}
	bool m_bIsCheckTimeOut;
	time_t m_nLastSend;
	CPackage m_pck;
};

class CSUdp
{
public:
	CSUdp(){
		InitWs2_32();
	};
	bool CreateClient(const char* szIp, short nPort);
	bool CreateServer(const char* szIp, short nPort);
	int Send(const char* buf, int nLen, sockaddr_in si = {});
	int Recv(char* buf, int nLen, sockaddr_in si = {});
	bool Connect();
	sockaddr_in* GetSrc();
	sockaddr_in Accept();
	void Close();
private:
	void InitWs2_32(); 
	void WorkThread();
	void Log(const char* szBuf)
	{
		OutputDebugString(szBuf);
	}

	// 变参模板
	template<typename... ARGS>
	void Log(const char* szFmt, ARGS... args)
	{
		char szBuf[0x1000] = {};
		wsprintf(szBuf, szFmt, args...);
		Log(szBuf);
	}
private:
	SOCKET m_sock;
	sockaddr_in m_siSrc;
	std::map<std::string, sockaddr_in> m_siDsts; // 目标地址

	bool m_bIsRunning;
	bool m_bIsClient;
	std::map<std::string, int> m_nNextSendSeq;
	std::map<std::string, int> m_nNextRecvSeq;

	std::map<std::string, std::mutex> m_mtxSendList;
	std::map<std::string, std::mutex> m_mtxRecvBuf;


	std::map<std::string, std::list<PackageInfo>> m_mpSendLists;
	std::map<std::string, std::map<int, PackageInfo>> m_mpSends;
	std::map<std::string, std::map<int, CPackage>> m_mpRecvs;
	std::map<std::string, std::vector<char>> m_vctBufs;
};