

#include <iostream>
#include <io.h>
#include <direct.h>
#include <string>
#include <regex>
using namespace std;
#include "./curl/curl.h"
#pragma comment(lib, "libcurl.lib")

string UTF8ToGBK(const std::string& strUTF8);

void DownloadFile(std::string startPage, std::string sExpendUrl, std::string sName, std::string sDirName);

void SearchAllDir(std::string startPage, std::string sDirName);

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    std::string& strRes = *(std::string*)userdata;
    strRes += ptr;

    return size * nmemb;//返回处理的字节数
}


int main()
{
    //全局的，只用调用一次就可以了
    curl_global_init(CURL_GLOBAL_ALL);

    string startPage = "https://opensource.apple.com/source/objc4/objc4-818.2/";
    string root = ".\\objc4-818.2";
    if (-1 == _access(root.c_str(), 0))
    {
        auto nRet = _mkdir(root.c_str());
    }
    SearchAllDir(startPage, root);


    //全局的，只用调用一次就可以了
    curl_global_cleanup();
}

void SearchAllDir(std::string startPage, std::string sDirName)
{
    //初始化请求
    CURL* curl = curl_easy_init();

    //不验证
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    //设置收包回调函数
    string strResponce;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponce); //回调函数的自定义参数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);//回调函数


                                                                 //设置url
    curl_easy_setopt(curl, CURLOPT_URL, startPage.c_str());

    //设置包头
    struct curl_slist* chunk = NULL;
    chunk = curl_slist_append(chunk, "Host: opensource.apple.com");
    chunk = curl_slist_append(chunk, "Connection: keep-alive");
    chunk = curl_slist_append(chunk, "sec-ch-ua: \"Not ? A_Brand\";v=\"8\", \"Chromium\";v=\"108\", \"Google Chrome\";v=\"108\"");
    chunk = curl_slist_append(chunk, "sec-ch-ua-mobile: ?0");
    chunk = curl_slist_append(chunk, "sec-ch-ua-platform: \"Windows\"");
    chunk = curl_slist_append(chunk, "Upgrade-Insecure-Requests: 1");
    chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36");
    chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
    chunk = curl_slist_append(chunk, "Sec-Fetch-Site: none");
    chunk = curl_slist_append(chunk, "Sec-Fetch-Mode: navigate");
    chunk = curl_slist_append(chunk, "Sec-Fetch-User: ?1");
    chunk = curl_slist_append(chunk, "Sec-Fetch-Dest: document");
    chunk = curl_slist_append(chunk, "Accept-Language: zh-CN,zh;q=0.9");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    //发送包
    CURLcode code = curl_easy_perform(curl);

    string strHtml = UTF8ToGBK(strResponce);
    regex reg("<td valign=\"top\"><a href=\".*?\"><img src=\".*?\" alt=(.*?) height=\".*?\"></a></td><td><a href=\"(.*?)\">(.*?)</a></td><td align=\"right\">.*?</td>");
    smatch m;
    string::const_iterator pos = strHtml.begin();
    string::const_iterator end = strHtml.end();
    for (; regex_search(pos, end, m, reg); pos = m.suffix().first)
    {
        cout << "文件扫描------------------" << endl;
        string sType = m.str(1);
        string sExpendUrl = m.str(2);
        string sName = m.str(3);
        cout << sType << endl;
        cout << sExpendUrl << endl;
        cout << sName << endl;
        if (sType == "[DIR]" || sType == "\"[DIR]\"")
        {
            if (sName != "Parent Directory")
            {
                string filePath = (sDirName + "\\") + sName.substr(0, sName.size() - 1);
                if (-1 == _access(filePath.c_str(), 0))
                {
                    auto nRet = _mkdir(filePath.c_str());
                    cout << "创建" << sName << "目录" << endl;
                }
                string nextPage = startPage + sExpendUrl;
                SearchAllDir(nextPage, filePath);
            }
        }
        else
        {
            DownloadFile(startPage, sExpendUrl, sName, sDirName);
        }
    }

    //清理
    curl_easy_cleanup(curl);
}

void DownloadFile(std::string startPage, std::string sExpendUrl, std::string sName, std::string sDirName)
{
    CURL* curl = curl_easy_init();

    //不验证
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    //设置收包回调函数
    string strResponce;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponce); //回调函数的自定义参数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);//回调函数


    //设置url
    string subPage;
    if (startPage.at(startPage.size() - 1) == '/')
    {
        subPage = (startPage + sExpendUrl);
    }
    else
    {
        subPage = (startPage + "/") += sExpendUrl;
    }
    curl_easy_setopt(curl, CURLOPT_URL, subPage.c_str());

    //设置包头
    struct curl_slist* chunk = NULL;
    chunk = curl_slist_append(chunk, "Host: opensource.apple.com");
    chunk = curl_slist_append(chunk, "Connection: keep-alive");
    chunk = curl_slist_append(chunk, "sec-ch-ua: \"Not ? A_Brand\";v=\"8\", \"Chromium\";v=\"108\", \"Google Chrome\";v=\"108\"");
    chunk = curl_slist_append(chunk, "sec-ch-ua-mobile: ?0");
    chunk = curl_slist_append(chunk, "sec-ch-ua-platform: \"Windows\"");
    chunk = curl_slist_append(chunk, "Upgrade-Insecure-Requests: 1");
    chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36");
    chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
    chunk = curl_slist_append(chunk, "Sec-Fetch-Site: none");
    chunk = curl_slist_append(chunk, "Sec-Fetch-Mode: navigate");
    chunk = curl_slist_append(chunk, "Sec-Fetch-User: ?1");
    chunk = curl_slist_append(chunk, "Sec-Fetch-Dest: document");
    chunk = curl_slist_append(chunk, "Accept-Language: zh-CN,zh;q=0.9");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    //发送包
    CURLcode code = curl_easy_perform(curl);

    string strHtml = UTF8ToGBK(strResponce);
    regex reg("<a href=\"(.*?)\">plain text</a>");
    smatch m;
    if (regex_search(strHtml, m, reg))
    {
        string page = m.str(1);
        DownloadFile(startPage, page, sName, sDirName);
    }
    else
    {
        FILE* file = fopen(((sDirName + "\\") + sName).c_str(), "wb+");
        fwrite(strHtml.c_str(), 1, strHtml.size(), file);
        fclose(file);
    }
    curl_easy_cleanup(curl);
}

string UTF8ToGBK(const std::string& strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    TCHAR* wszGBK = new TCHAR[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    std::string strTemp(szGBK);
    delete[]szGBK;
    delete[]wszGBK;
    return strTemp;
}
