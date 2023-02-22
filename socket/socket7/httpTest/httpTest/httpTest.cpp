#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <regex>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
void InitWs2_32();

string Utf8ToGbk(const char* src_str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if (wszGBK) delete[] wszGBK;
    if (szGBK) delete[] szGBK;
    return strTemp;
}

int main()
{
    InitWs2_32();
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in si = {};
    si.sin_family = AF_INET;
    si.sin_port = htons(80);
    si.sin_addr.S_un.S_addr = inet_addr("220.181.174.98");

    if (connect(sock, (sockaddr*)&si, sizeof(si)) == SOCKET_ERROR)
    {
        cout << "failed" << endl;
    }
    // 开启非阻塞模式
    u_long nEnableNoblock = 1;
    ioctlsocket(sock, FIONBIO, &nEnableNoblock);
    const char* szHeader =
        "GET / HTTP/1.1\r\n"
        "Host: www.265.com\r\n"
        "Connection: keep-alive\r\n"
        "Pragma: no-cache\r\n"
        "Cache-Control: no-cache\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
        "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n";
    
    int nRet = send(sock, szHeader, strlen(szHeader), 0);
    
    char* szRecv = nullptr;
    int nPage = 1;
    Sleep(300);
    do
    {
        int curSize = 0x1000 * nPage;
        int preSize = 0x1000 * (nPage - 1);
        if (szRecv != nullptr)
        {
            char* tempRecv = szRecv;
            szRecv = new char[curSize];
            memcpy_s(szRecv, preSize, tempRecv, preSize);
            delete[] tempRecv;
        }
        else
        {
            szRecv = new char[curSize];
        }
        nRet = recv(sock, szRecv + preSize, 0x1000, 0);
        nPage++;
    } while (nRet != -1);
    
    string recvHtml = Utf8ToGbk(szRecv);
    delete[] szRecv;

    int nEnd = recvHtml.find_last_of("</html>");
    recvHtml = recvHtml.substr(0, nEnd + 1);
    //cout << recvHtml << endl;

    std::regex reg("<a style=\".*?\" href=\"(.*?)\" site-c-action=\"c-famous-click\" target=\"_blank\">(.*?)</a>");
    std::smatch m;
    string::const_iterator pos = recvHtml.begin();
    string::const_iterator end = recvHtml.end();
    for (;std::regex_search(pos, end, m, reg); pos = m.suffix().first)
    {
        std::cout << "----------------提取名站导航" << std::endl;
        std::cout << m.str() << std::endl;
        std::cout << m.str(1) << std::endl;
        std::cout << m.str(2) << std::endl;
    }
    closesocket(sock);
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
