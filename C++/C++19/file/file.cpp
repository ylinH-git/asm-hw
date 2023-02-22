// file.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    std::fstream fs;
    fs.open(".//countuse.cpp", std::fstream::in | std::fstream::out | std::fstream::app);
    int nRowCount = 0;
    int nRowCountEmpty = 0;
    int nRowCountComment = 0;
    if (fs.is_open())
    {
        // 清空开头的3个前置字符
        fs.get();
        fs.get();
        fs.get();
        bool isCommentFlag = false;
        while (!fs.eof()) {
            char str[1000];
            fs.getline(str, sizeof(str));
            nRowCount++;
            if (!strlen(str))
            {
                nRowCountEmpty++;
            }
            string s = str;
            s.erase(0, s.find_first_not_of(" "));
            if (s.substr(0, 2) == "//")
            {
                nRowCountComment++;
            }
            if (s.substr(0, 2) == "/*")
            {
                isCommentFlag = true;
            }
            int index = s.rfind("*/");
            if (index != -1)
            {
                if (s.substr(index, 2) == "*/")
                {
                    nRowCountComment++;
                    isCommentFlag = false;
                }
            }
            if (isCommentFlag)
            {
                nRowCountComment++;
            }
        }
    }
    cout << "总行数： " << nRowCount << endl;
    cout << "空行数： " << nRowCountEmpty << endl;
    cout << "纯注释行数： " << nRowCountComment << endl;
    fs.close();
}
