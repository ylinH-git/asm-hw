// removeDll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#define DEF_DLL_NAME ("myDll.dll")      //定义要卸载的dll名

DWORD FindProcessID(LPCTSTR szProcessName)
{
    DWORD dwPID = 0xFFFFFFFF;                                //初始化PID为0xFFFFFFFF
    HANDLE hSnapShot = INVALID_HANDLE_VALUE; //初始化快照句柄为INVALID_HANDLE_VALUE
    PROCESSENTRY32 pe;    //定义一个存放 快照进程信息 的一个结构体
    //1.获取当前系统进程快照
    pe.dwSize = sizeof(PROCESSENTRY32);
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    //2.查找进程
    Process32First(hSnapShot, &pe);
    do
    {
        if (!strcmp(szProcessName, (LPCTSTR)pe.szExeFile))     //比较进程名
        {
            dwPID = pe.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnapShot, &pe));     //循环查找
    //关闭句柄并返回
    CloseHandle(hSnapShot);
    return dwPID;
}

BOOL EjectDll(DWORD dwPID, LPCTSTR szDllName)
{
    BOOL bMore = FALSE, bFound = FALSE;
    HANDLE hSnapshot, hProcess, hThread;
    HMODULE hModule = NULL;
    MODULEENTRY32 me = { sizeof(me) }; //定义一个用于储存模块快照的结构体
    LPTHREAD_START_ROUTINE pThreadProc;

    //1.
    //dwPID=notepad进程ID
    //使用TH32CS_SNAPMODULE参数
    //获取加载到notepad进程的DLL名称
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

    //此函数检索与进程相关联的第一个模块的信息
    bMore = Module32First(hSnapshot, &me);

    for (; bMore; bMore = Module32Next(hSnapshot, &me))   //bMore用于判断该进程的模块快照是否还有，bFound用于判断是否找到了我们想要卸载的dll模块
    {
        if (!strcmp((LPCTSTR)me.szModule, szDllName) || !strcmp((LPCTSTR)me.szExePath, szDllName))
        {
            bFound = TRUE;
            break;
        }
    }

    if (!bFound)
    {
        CloseHandle(hSnapshot);
        return FALSE;
    }
    //2. 通过进程PID获取进程句柄
    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
    {
        return FALSE;
    }
    //3. 获取FreeLibrary函数的地址
    hModule = GetModuleHandle("kernel32.dll");
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
    //4.创建线程来执行FreeLibrary(modBaseAddr要卸载的dll模块基址)
    hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, me.modBaseAddr, 0, NULL);

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);
    CloseHandle(hSnapshot);

    return TRUE;
}


int main()
{
    DWORD dwPID = 0xFFFFFFFF;    //进程的pid是DWORD格式的，先初始化为0xFFFFFFFF
    dwPID = FindProcessID("winmine.exe");
    Sleep(100);
    EjectDll(dwPID, DEF_DLL_NAME);
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
