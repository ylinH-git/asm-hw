#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <thread>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
void InitWs2_32();
//网际校验和是被校验数据16位值的反码和(ones-complement sum)
WORD CalcCheckSum(IN unsigned short* addr, IN int len)
{
    int		nleft = len;
    int		sum = 0;
    unsigned short* w = addr;
    unsigned short answer = 0;

    //计算和
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    //加上奇数
    if (nleft == 1) {
        *(unsigned char*)(&answer) = *(unsigned char*)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff); //高16位加上低16位
    sum += (sum >> 16); //加上进位
    answer = ~sum; //取反

    return (answer);
}


#pragma pack(push)
#pragma pack(1)
struct Icmp
{
    BYTE type;
    BYTE code;
    WORD checksum;
    WORD id;
    WORD seq;
    BYTE data[32];
};
#pragma pack(pop)


int main(int argc, char** argv)
{
    InitWs2_32();
    if (argv[1] != nullptr)
    {
        hostent* pHost = gethostbyname(argv[1]);
        // 因为这里是网络层协议，所以需要使用原始套接字（SOCK_RAW）
        SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

        // 发送icmp包
        Icmp icmp = {};
        icmp.type = 8;
        icmp.code = 0;
        icmp.checksum = 0;
        icmp.id = 0x5678;
        icmp.seq = 0x1234;
        memcpy_s(icmp.data, sizeof(icmp.data), "abcdefghijklmnopqrstuvwxyzabcde", sizeof(icmp.data));
        icmp.checksum = CalcCheckSum((USHORT*)&icmp, sizeof(icmp));

        sockaddr_in si = {};
        si.sin_family = AF_INET;
        si.sin_port = 0; 
        si.sin_addr.S_un.S_addr = *(ULONG*)pHost->h_addr_list[0];
        cout << "正在Ping " << argv[1] << " [ " << inet_ntoa(si.sin_addr)  << " ] " << "具有32个字节的数据" << endl;

        for (int i = 0; i < 4; i++)
        {
            int nRet = sendto(sock, (char*)&icmp, sizeof(icmp), 0, (sockaddr*)&si, sizeof(si));

            char recvData[0x1000] = {};
            int nSiSize = sizeof(si);
            
            nRet = recvfrom(sock, (char*)recvData, sizeof(recvData), 0, (sockaddr*)&si, &nSiSize);

            Icmp* pIcmp = (Icmp*)(recvData + 0x14);
            cout << "来自 " << inet_ntoa(si.sin_addr) << " 的回复：" << "字节：" << strlen((char*)pIcmp->data) + 1 << endl;
        }
    }
}


void InitWs2_32()
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
