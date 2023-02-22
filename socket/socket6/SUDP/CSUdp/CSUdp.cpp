// CSUdp.cpp 
//

#include "CSUdp.h"
#include <iostream>

bool CSUdp::CreateClient(const char* szIp, short nPort)
{
    m_sock= socket(AF_INET/*ipv4*/, SOCK_DGRAM/*数据报*/, IPPROTO_UDP);
    if (m_sock == INVALID_SOCKET)
    {
        return false;
    }

    // 开启非阻塞模式
    u_long nEnableNoblock = 1;
    ioctlsocket(m_sock, FIONBIO, &nEnableNoblock);

    sockaddr_in siDst = {};
    siDst.sin_family = AF_INET;
    siDst.sin_port = htons(nPort);
    siDst.sin_addr.S_un.S_addr = inet_addr(szIp);
    std::string dstStr = inet_ntoa(siDst.sin_addr);
    (dstStr += ":") += std::to_string(ntohs(siDst.sin_port));
    m_siDsts[dstStr] = siDst;
    m_nNextSendSeq[dstStr] = 0;
    m_nNextRecvSeq[dstStr] = 0;

    // 随机端口号
    m_siSrc.sin_family = AF_INET;
    m_siSrc.sin_port = 0;
    m_siSrc.sin_addr.S_un.S_addr = inet_addr(szIp);
    int srcLen = sizeof(m_siSrc);
    if (bind(m_sock, (sockaddr*)&m_siSrc, srcLen) == SOCKET_ERROR)
    {
        return false;
    }

    m_bIsRunning = true;
    m_bIsClient = true;
    std::thread(&CSUdp::WorkThread, this).detach();
    getsockname(m_sock, (sockaddr*)&m_siSrc, &srcLen);
    Log("[SUdp]: create client:%s:%d", inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port));
	return true;
}

bool CSUdp::CreateServer(const char* szIp, short nPort)
{
    m_sock = socket(AF_INET/*ipv4*/, SOCK_DGRAM/*数据报*/, IPPROTO_UDP);
    if (m_sock == INVALID_SOCKET)
    {
        return false;
    }

    // 开启非阻塞模式
    u_long nEnableNoblock = 1;
    ioctlsocket(m_sock, FIONBIO, &nEnableNoblock);


    m_siSrc.sin_family = AF_INET;
    m_siSrc.sin_port = htons(nPort);
    m_siSrc.sin_addr.S_un.S_addr = inet_addr(szIp);
    if (bind(m_sock, (sockaddr*)&m_siSrc, sizeof(m_siSrc)) == SOCKET_ERROR)
    {
        return false;
    }
    m_bIsRunning = true;
    m_bIsClient = false;
    std::thread(&CSUdp::WorkThread, this).detach();
    Log("[SUdp]: create server:%s:%d", inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port));
	return true;
}

int CSUdp::Send(const char* buf, int nLen, sockaddr_in si)
{
    // 拆包
    // 计算拆包数量
    std::string siStr = inet_ntoa(si.sin_addr);
    (siStr += ":") += std::to_string(ntohs(si.sin_port));
    std::string realSi = m_bIsClient ? m_siDsts.begin()->first : siStr;
    int nCnt = nLen % MSS > 0 ? (nLen / MSS) + 1 : nLen / MSS;
    for (size_t i = 0; i < nCnt; i++)
    {
        std::lock_guard<std::mutex> lock(m_mtxSendList[realSi]);
        if (i == (nCnt - 1))
        {
            CPackage pck((char)PackFlag::PF_PSH, m_nNextSendSeq[realSi], buf + i * MSS, nLen - (i * MSS));
            m_mpSendLists[realSi].emplace_back(false, 0, pck);
        }
        else
        {
            CPackage pck((char)PackFlag::PF_PSH, m_nNextSendSeq[realSi], buf + i * MSS, MSS);
            m_mpSendLists[realSi].emplace_back(false, 0, pck);
        }
        ++m_nNextSendSeq[realSi];
        Log("[SUdp]: %s:%d Send: split to list, next seq: %d", inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port), m_nNextSendSeq[realSi]);
    }
	return 0;
}

int CSUdp::Recv(char* buf, int nLen, sockaddr_in si)
{
    std::string siStr = inet_ntoa(si.sin_addr);
    (siStr += ":") += std::to_string(ntohs(si.sin_port));
    std::string realSi = m_bIsClient ? m_siDsts.begin()->first : siStr;

    std::lock_guard<std::mutex> lock(m_mtxRecvBuf[realSi]);
    int nAvailSize = m_vctBufs[realSi].size() > nLen ? nLen : m_vctBufs[realSi].size();
    
    RtlCopyMemory(buf, m_vctBufs[realSi].data(), nAvailSize);

    m_vctBufs[realSi].erase(m_vctBufs[realSi].begin(), m_vctBufs[realSi].begin() + nAvailSize);
    Log("[SUdp]: %s:%d Recv: vctBuf to buf, %d", inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port), nAvailSize);
	return nAvailSize;
}

bool CSUdp::Connect()
{
    CPackage pckSyn((char)PackFlag::PF_SYN, m_nNextSendSeq[m_siDsts.begin()->first]);
    sendto(m_sock, (char*)&pckSyn, HDRSIZE + pckSyn.m_nLen, 0, (sockaddr*)&m_siDsts.begin()->second, sizeof(m_siDsts.begin()->second));
    while (true)
    {
        CPackage pck;
        sockaddr_in si = {};
        int nSiSize = sizeof(si);
        int nRet = recvfrom(m_sock, (char*)&pck, sizeof(pck), 0, (sockaddr*)&si, &nSiSize);
        if (nRet == 0 || nRet == SOCKET_ERROR)
        {
            continue;
        }

        // 收包逻辑
        switch (pck.m_nFlag)
        {
        case (char)PackFlag::PF_ACK:
        {
            std::string siStr = inet_ntoa(si.sin_addr);
            (siStr += ":") += std::to_string(ntohs(si.sin_port));
            m_siDsts[siStr] = si;
            Log("[SUdp]: connect success %s:%d", inet_ntoa(si.sin_addr), ntohs(si.sin_port));
            return true;
        }
        default:
            continue;
        }
    }
    return false;
}

sockaddr_in* CSUdp::GetSrc()
{
    return &m_siSrc;
}

sockaddr_in CSUdp::Accept()
{
    CPackage pck;
    sockaddr_in si = {};
    while (true)
    {
        int nSiSize = sizeof(si);
        int nRet = recvfrom(m_sock, (char*)&pck, sizeof(pck), MSG_PEEK, (sockaddr*)&si, &nSiSize);
        if (nRet == 0 || nRet == SOCKET_ERROR)
        {
            continue;
        }
        std::string siStr = inet_ntoa(si.sin_addr);
        (siStr += ":") += std::to_string(ntohs(si.sin_port));
        // 收包逻辑
        switch (pck.m_nFlag)
        {
        case (char)PackFlag::PF_SYN:
        {

            m_siDsts[siStr] = si;
            m_nNextSendSeq[siStr] = 0;
            m_nNextRecvSeq[siStr] = 0;
            // 回复ACK
            CPackage pckAck((char)PackFlag::PF_ACK, pck.m_nSeq);
            sendto(m_sock, (char*)&pckAck, HDRSIZE + pckAck.m_nLen, 0, (sockaddr*)&si, sizeof(si));
            Log("[SUdp]: accept success %s:%d", inet_ntoa(si.sin_addr), ntohs(si.sin_port));
            return si;
        }
        default:
            continue;
        }
    }
}

void CSUdp::Close()
{
    m_bIsRunning = false;
    closesocket(m_sock);
}

int  CPackage::Crc32(const char* pData, int nLen)
{
    static const unsigned long crc_table[256] = {
                0x00000000UL, 0x77073096UL, 0xee0e612cUL, 0x990951baUL, 0x076dc419UL,
                0x706af48fUL, 0xe963a535UL, 0x9e6495a3UL, 0x0edb8832UL, 0x79dcb8a4UL,
                0xe0d5e91eUL, 0x97d2d988UL, 0x09b64c2bUL, 0x7eb17cbdUL, 0xe7b82d07UL,
                0x90bf1d91UL, 0x1db71064UL, 0x6ab020f2UL, 0xf3b97148UL, 0x84be41deUL,
                0x1adad47dUL, 0x6ddde4ebUL, 0xf4d4b551UL, 0x83d385c7UL, 0x136c9856UL,
                0x646ba8c0UL, 0xfd62f97aUL, 0x8a65c9ecUL, 0x14015c4fUL, 0x63066cd9UL,
                0xfa0f3d63UL, 0x8d080df5UL, 0x3b6e20c8UL, 0x4c69105eUL, 0xd56041e4UL,
                0xa2677172UL, 0x3c03e4d1UL, 0x4b04d447UL, 0xd20d85fdUL, 0xa50ab56bUL,
                0x35b5a8faUL, 0x42b2986cUL, 0xdbbbc9d6UL, 0xacbcf940UL, 0x32d86ce3UL,
                0x45df5c75UL, 0xdcd60dcfUL, 0xabd13d59UL, 0x26d930acUL, 0x51de003aUL,
                0xc8d75180UL, 0xbfd06116UL, 0x21b4f4b5UL, 0x56b3c423UL, 0xcfba9599UL,
                0xb8bda50fUL, 0x2802b89eUL, 0x5f058808UL, 0xc60cd9b2UL, 0xb10be924UL,
                0x2f6f7c87UL, 0x58684c11UL, 0xc1611dabUL, 0xb6662d3dUL, 0x76dc4190UL,
                0x01db7106UL, 0x98d220bcUL, 0xefd5102aUL, 0x71b18589UL, 0x06b6b51fUL,
                0x9fbfe4a5UL, 0xe8b8d433UL, 0x7807c9a2UL, 0x0f00f934UL, 0x9609a88eUL,
                0xe10e9818UL, 0x7f6a0dbbUL, 0x086d3d2dUL, 0x91646c97UL, 0xe6635c01UL,
                0x6b6b51f4UL, 0x1c6c6162UL, 0x856530d8UL, 0xf262004eUL, 0x6c0695edUL,
                0x1b01a57bUL, 0x8208f4c1UL, 0xf50fc457UL, 0x65b0d9c6UL, 0x12b7e950UL,
                0x8bbeb8eaUL, 0xfcb9887cUL, 0x62dd1ddfUL, 0x15da2d49UL, 0x8cd37cf3UL,
                0xfbd44c65UL, 0x4db26158UL, 0x3ab551ceUL, 0xa3bc0074UL, 0xd4bb30e2UL,
                0x4adfa541UL, 0x3dd895d7UL, 0xa4d1c46dUL, 0xd3d6f4fbUL, 0x4369e96aUL,
                0x346ed9fcUL, 0xad678846UL, 0xda60b8d0UL, 0x44042d73UL, 0x33031de5UL,
                0xaa0a4c5fUL, 0xdd0d7cc9UL, 0x5005713cUL, 0x270241aaUL, 0xbe0b1010UL,
                0xc90c2086UL, 0x5768b525UL, 0x206f85b3UL, 0xb966d409UL, 0xce61e49fUL,
                0x5edef90eUL, 0x29d9c998UL, 0xb0d09822UL, 0xc7d7a8b4UL, 0x59b33d17UL,
                0x2eb40d81UL, 0xb7bd5c3bUL, 0xc0ba6cadUL, 0xedb88320UL, 0x9abfb3b6UL,
                0x03b6e20cUL, 0x74b1d29aUL, 0xead54739UL, 0x9dd277afUL, 0x04db2615UL,
                0x73dc1683UL, 0xe3630b12UL, 0x94643b84UL, 0x0d6d6a3eUL, 0x7a6a5aa8UL,
                0xe40ecf0bUL, 0x9309ff9dUL, 0x0a00ae27UL, 0x7d079eb1UL, 0xf00f9344UL,
                0x8708a3d2UL, 0x1e01f268UL, 0x6906c2feUL, 0xf762575dUL, 0x806567cbUL,
                0x196c3671UL, 0x6e6b06e7UL, 0xfed41b76UL, 0x89d32be0UL, 0x10da7a5aUL,
                0x67dd4accUL, 0xf9b9df6fUL, 0x8ebeeff9UL, 0x17b7be43UL, 0x60b08ed5UL,
                0xd6d6a3e8UL, 0xa1d1937eUL, 0x38d8c2c4UL, 0x4fdff252UL, 0xd1bb67f1UL,
                0xa6bc5767UL, 0x3fb506ddUL, 0x48b2364bUL, 0xd80d2bdaUL, 0xaf0a1b4cUL,
                0x36034af6UL, 0x41047a60UL, 0xdf60efc3UL, 0xa867df55UL, 0x316e8eefUL,
                0x4669be79UL, 0xcb61b38cUL, 0xbc66831aUL, 0x256fd2a0UL, 0x5268e236UL,
                0xcc0c7795UL, 0xbb0b4703UL, 0x220216b9UL, 0x5505262fUL, 0xc5ba3bbeUL,
                0xb2bd0b28UL, 0x2bb45a92UL, 0x5cb36a04UL, 0xc2d7ffa7UL, 0xb5d0cf31UL,
                0x2cd99e8bUL, 0x5bdeae1dUL, 0x9b64c2b0UL, 0xec63f226UL, 0x756aa39cUL,
                0x026d930aUL, 0x9c0906a9UL, 0xeb0e363fUL, 0x72076785UL, 0x05005713UL,
                0x95bf4a82UL, 0xe2b87a14UL, 0x7bb12baeUL, 0x0cb61b38UL, 0x92d28e9bUL,
                0xe5d5be0dUL, 0x7cdcefb7UL, 0x0bdbdf21UL, 0x86d3d2d4UL, 0xf1d4e242UL,
                0x68ddb3f8UL, 0x1fda836eUL, 0x81be16cdUL, 0xf6b9265bUL, 0x6fb077e1UL,
                0x18b74777UL, 0x88085ae6UL, 0xff0f6a70UL, 0x66063bcaUL, 0x11010b5cUL,
                0x8f659effUL, 0xf862ae69UL, 0x616bffd3UL, 0x166ccf45UL, 0xa00ae278UL,
                0xd70dd2eeUL, 0x4e048354UL, 0x3903b3c2UL, 0xa7672661UL, 0xd06016f7UL,
                0x4969474dUL, 0x3e6e77dbUL, 0xaed16a4aUL, 0xd9d65adcUL, 0x40df0b66UL,
                0x37d83bf0UL, 0xa9bcae53UL, 0xdebb9ec5UL, 0x47b2cf7fUL, 0x30b5ffe9UL,
                0xbdbdf21cUL, 0xcabac28aUL, 0x53b39330UL, 0x24b4a3a6UL, 0xbad03605UL,
                0xcdd70693UL, 0x54de5729UL, 0x23d967bfUL, 0xb3667a2eUL, 0xc4614ab8UL,
                0x5d681b02UL, 0x2a6f2b94UL, 0xb40bbe37UL, 0xc30c8ea1UL, 0x5a05df1bUL,
                0x2d02ef8dUL
    };

    unsigned long nRes = 0xffffffff;
    for (int i = 0; i < nLen; i++) {
        nRes = crc_table[(nRes ^ pData[i]) & 0xff] ^ (nRes >> 8);
    }
    return nRes ^ 0xffffffff;
}

bool CPackage::IsCheckOK()
{
    return m_nChek == Crc32(m_buf, m_nLen);
}


void CSUdp::InitWs2_32()
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

void CSUdp::WorkThread()
{
    while (m_bIsRunning)
    {
        for (auto& pSi : m_siDsts)
        {
            // 发送逻辑
            
            // 窗口内存在需要发送的包则遍历监听
            for (auto& pi : m_mpSends[pSi.first])
            {
                if (!pi.second.m_bIsCheckTimeOut)
                {
                    sendto(m_sock, (char*)&pi.second.m_pck, HDRSIZE + pi.second.m_pck.m_nLen, 0, (sockaddr*)&pSi.second, sizeof(pSi.second));
                    pi.second.m_bIsCheckTimeOut = true;
                    pi.second.m_nLastSend = clock();
                }
                else
                {
                    if (clock() - pi.second.m_nLastSend > RESENDTIMEOUT)
                    {
                        sendto(m_sock, (char*)&pi.second.m_pck, HDRSIZE + pi.second.m_pck.m_nLen, 0, (sockaddr*)&pSi.second, sizeof(pSi.second));
                        pi.second.m_bIsCheckTimeOut = true;
                        pi.second.m_nLastSend = clock();
                    }
                }
                Log("[SUdp]: %s:%d send to %s:%d, seq:%d",
                    inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port),
                    inet_ntoa(pSi.second.sin_addr), ntohs(pSi.second.sin_port),
                    pi.second.m_pck.m_nSeq
                );
            }

         
            m_mtxSendList[pSi.first].lock();
            // 生成窗口，按批次发送信息
            while ((m_mpSends[pSi.first].size() < WINDOWSIZE) && !m_mpSendLists[pSi.first].empty())
            {
                PackageInfo pckInfo = m_mpSendLists[pSi.first].front();
                (m_mpSends[pSi.first])[pckInfo.m_pck.m_nSeq] = pckInfo;
                (m_mpSendLists[pSi.first]).pop_front();
            }
            m_mtxSendList[pSi.first].unlock();
          

            // 收包
            // 处理收包后的逻辑
            if (m_mpRecvs[pSi.first].find(m_nNextRecvSeq[pSi.first]) != m_mpRecvs[pSi.first].end())
            {
                auto& pck = (m_mpRecvs[pSi.first])[m_nNextRecvSeq[pSi.first]];
                m_mtxRecvBuf[pSi.first].lock();
                m_vctBufs[pSi.first].insert(m_vctBufs[pSi.first].end(), pck.m_buf, pck.m_buf + pck.m_nLen);
                m_mtxRecvBuf[pSi.first].unlock();
                m_mpRecvs[pSi.first].erase(m_nNextRecvSeq[pSi.first]);
                Log("[SUdp]: %s:%d recv from %s:%d, put seq:%d info into buf",
                    inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port),
                    inet_ntoa(pSi.second.sin_addr), ntohs(pSi.second.sin_port),
                    pck.m_nSeq
                );
                m_nNextRecvSeq[pSi.first]++;
            }


            CPackage pck;
            int nSiSize = sizeof(pSi.second);
            int nRet = recvfrom(m_sock, (char*)&pck, sizeof(pck), 0, (sockaddr*)&pSi.second, &nSiSize);
            if (nRet == 0 || nRet == SOCKET_ERROR)
            {
                continue;
            }

            // 收包逻辑
            switch (pck.m_nFlag)
            {
            case (char)PackFlag::PF_ACK:
            {
                // 收到ack，删除收包容器中的相同序号包
                std::lock_guard<std::mutex> lock(m_mtxSendList[pSi.first]);
                m_mpSends[pSi.first].erase(pck.m_nSeq);
                Log("[SUdp]: %s:%d recv ack from %s:%d, seq:%d",
                    inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port),
                    inet_ntoa(pSi.second.sin_addr), ntohs(pSi.second.sin_port),
                    pck.m_nSeq
                );
                break;
            }
            case (char)PackFlag::PF_PSH:
            {
                // 校验
                // 校验失败
                if (!pck.IsCheckOK())
                {
                    break;
                }
                // 回复ACK
                CPackage pckAck((char)PackFlag::PF_ACK, pck.m_nSeq);
                sendto(m_sock, (char*)&pckAck, HDRSIZE + pckAck.m_nLen, 0, (sockaddr*)&pSi.second, sizeof(pSi.second));


                // 放入收包容器
                (m_mpRecvs[pSi.first])[pck.m_nSeq] = pck;
                Log("[SUdp]: %s:%d recv info from %s:%d, seq:%d, put into recv map",
                    inet_ntoa(m_siSrc.sin_addr), ntohs(m_siSrc.sin_port),
                    inet_ntoa(pSi.second.sin_addr), ntohs(pSi.second.sin_port),
                    pck.m_nSeq
                );
                break;
            }
            default:
                break;
            }
        }
    }
}
