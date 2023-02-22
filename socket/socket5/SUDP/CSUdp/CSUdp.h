#pragma once
#include <map>
#include <thread>
#include <mutex>
#include <vector>

#include <WinSock2.h>
#include <Windows.h>

#define MTU 1472
#define HDRSIZE 11
#define MSS (MTU-HDRSIZE) // 最大数据长度
#define RESENDTIMEOUT 100

enum class PackFlag
{
	PF_PSH = 1,
	PF_ACK = 2
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
	bool m_bIsCheckTimeOut;
	time_t m_nLastSend;
	CPackage m_pck;
};

class CSUdp
{
public:
	CSUdp(): m_nNextSendSeq(0), m_bIsRunning(false), m_nNextRecvSeq(0){
		InitWs2_32();
	};
	bool CreateClient(const char* szIp, short nPort);
	bool CreateServer(const char* szIp, short nPort);
	int Send(const char* buf, int nLen);
	int Recv(char* buf, int nLen);
	void Close();
private:
	void InitWs2_32(); 
	void WorkThread();
private:
	SOCKET m_sock;
	sockaddr_in m_siDst; // 目标地址

	bool m_bIsRunning;
	int m_nNextSendSeq;
	int m_nNextRecvSeq;

	std::mutex m_mtxSendMap;
	std::mutex m_mtxRecvBuf;

	std::map<int, PackageInfo> m_mpSend; // 发包容器
	std::map<int, CPackage> m_mpRecv; // 收包容器
	std::vector<char> m_vctBuf; // 收包缓存区
};