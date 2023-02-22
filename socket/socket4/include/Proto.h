#pragma once
#include <Windows.h>
#include <memory>
#include <thread>;

using namespace std;

enum class PkgFlags
{
	SYN,
	ACK,
	FIN,
	SYNACK,
	FINACK
};

struct PkgHeader
{
	PkgHeader() {};
	PkgHeader(sockaddr_in src, sockaddr_in dst, PkgFlags flag, int seq, int segLen):
		m_addrSrc(src), m_addrDst(dst), m_flag(flag), m_seq(seq), m_segLen(segLen), m_check(0)
	{};
	sockaddr_in m_addrSrc;
	sockaddr_in m_addrDst;
	PkgFlags m_flag;
	int m_seq;
	int m_segLen;
	int m_check;
};
struct Package:public PkgHeader
{
	Package() {};
	Package(sockaddr_in src, sockaddr_in dst, PkgFlags flag, int seq, int segLen, char* seg):
		PkgHeader(src, dst, flag, seq, segLen)
	{
		if (segLen < 512 && seg != nullptr)
		{
			memcpy_s(m_seg, segLen, seg, segLen);
		}
		else if(segLen > 512)
		{
			printf("缓存不能超过512个字节");
		} 
	};
	char m_seg[512] = {};
};

int GenerateCheckSum(char* buf, int nLen)
{
	unsigned long cksum = 0;
	while (nLen > 1)
	{
		cksum += *buf++;
		nLen -= sizeof(USHORT);
	}
	if (nLen)
	{
		cksum += *(UCHAR*)buf;
	}
	while (cksum >> 16)
	{
		cksum = (cksum >> 16) + (cksum & 0xffff);
	}
	return ~cksum;
}


int ReadBytes(SOCKET sock, char* buf, int bufLen)
{
	int nByteReaded = 0;
	while (nByteReaded < bufLen)
	{
		sockaddr_in siRecv = {};
		int nformLen = sizeof(siRecv);
		int nRet = recvfrom(
			sock, 
			buf + nByteReaded, 
			bufLen - nByteReaded,
			0,
			(sockaddr*)&siRecv, 
			&nformLen
		);
		if (nRet == 0 || nRet == SOCKET_ERROR)
		{
			continue;
		}
	}
}

shared_ptr<PkgHeader> ReadPackage(SOCKET sock)
{
	// 先读包头
	PkgHeader hdr = {};
	int nRet = ReadBytes(sock, (char*)&hdr, sizeof(hdr));
	if (nRet == 0)
	{
		return 0;
	}
	// 再读数据
	shared_ptr<PkgHeader> ptrProto((PkgHeader*)new char[sizeof(hdr) + hdr.m_segLen]);
	memcpy(ptrProto.get(), &hdr, sizeof(hdr));
	nRet = ReadBytes(sock, (char*)ptrProto.get() + sizeof(hdr), hdr.m_segLen);
	if (nRet == 0)
	{
		return nullptr;
	}
	// 返回包
	return ptrProto;
}

int writePackagePure(SOCKET sock, PkgHeader* proto, BOOL& isWaiting)
{
	proto->m_check = GenerateCheckSum(((Package*)proto)->m_seg, proto->m_segLen);
	int nRet = sendto(sock,
		(char*)proto,
		sizeof(PkgHeader) + proto->m_segLen,
		0,
		(sockaddr*)&proto->m_addrDst,
		sizeof(proto->m_addrDst)
	);
	if (nRet == 0 || nRet == SOCKET_ERROR)
	{
		cout << "发送失败" << endl;
		return 0;
	}
	thread([&isWaiting, sock, proto, nRet]() {
		time_t now = time(NULL);
		while (true && nRet != SOCKET_ERROR)
		{
			time_t cur = time(NULL);
			if (cur - now > 3)
			{
				writePackagePure(sock, proto, isWaiting);
				break;
			}
		}
	}).detach();
	return nRet;
}
int WritePackage(SOCKET sock, PkgHeader* proto)
{
	BOOL isWaiting = TRUE;
	int nRet = writePackagePure(sock, proto, isWaiting);
	if (nRet == 0)
	{
		return 0;
	}
	while (isWaiting)
	{
		char szBuf[0x1000] = {};
		sockaddr_in siRecv = {};
		int nformLen = sizeof(siRecv);
		nRet = recvfrom(sock, szBuf, sizeof(szBuf),
			0,
			(sockaddr*)&siRecv, &nformLen);
		
		//shared_ptr<PkgHeader> ptrProto = ReadPackage(sock);
		/*if (ptrProto == nullptr)
		{
			isWaiting = FALSE;
			return 0;
		}*/
		if (nRet == 0 || nRet == SOCKET_ERROR)
		{
			cout << "接受出错" << endl;
		}
		else
		{
			Package* recvPkg = (Package*)szBuf;
			if (recvPkg->m_flag == PkgFlags::ACK && recvPkg->m_seq == (proto->m_seq + proto->m_segLen)) {
				isWaiting = FALSE;
				printf("收到来自 ip:%s port:%d 的数据: %s 当前序号：%d\r\n",
					inet_ntoa(siRecv.sin_addr),
					ntohs(siRecv.sin_port),
					szBuf,
					recvPkg->m_seq
				);
				break;
			}
		}
	}
	return nRet == SOCKET_ERROR ? 0 : nRet;
}