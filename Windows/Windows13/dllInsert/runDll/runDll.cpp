// runDll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
int main()
{
    // 找到扫雷窗口
    HWND hwndWinmine = FindWindow(NULL, "扫雷");
    // 获取进程ID
    DWORD proIdWinmine = 0;
    GetWindowThreadProcessId(hwndWinmine, &proIdWinmine);
    // 打开进程ID
    HANDLE proHdWinmine = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proIdWinmine);

    //获取当前进程中的LoadLibrary的地址（同一台电脑同一次启动时核心dll的地址都是一样的）
    HMODULE kernelHd = GetModuleHandle("kernel32");
    FARPROC pfnLoadLibrary = NULL;
    if (kernelHd) 
    {
        pfnLoadLibrary = GetProcAddress(kernelHd, "LoadLibraryA");
    }
    // 写入dll地址
    char szDllPath[] = { R"(E:\科锐逆向培训\作业\Windows\Windows13\dllInsert\Debug\myDll.dll)" };
    // 为三方程序申请一块内存并讲地址写入此内存内
    LPVOID pAddr = VirtualAllocEx(proHdWinmine, NULL, sizeof(szDllPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD dwBytesWrited = 0;
    if (pAddr)
    {
        WriteProcessMemory(proHdWinmine, pAddr, szDllPath, sizeof(szDllPath), &dwBytesWrited);
    }

    // 创建远程线程，加载dll
    if (pfnLoadLibrary) {
        HANDLE hThread = CreateRemoteThread(proHdWinmine, NULL, 0,
            (LPTHREAD_START_ROUTINE)pfnLoadLibrary, //远程线程执行LoadLibrary
            pAddr, //dll路径在目标进程中的地址
            0, NULL);
        if (hThread)
        {
            WaitForSingleObject(hThread, INFINITE);
        }
    }
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
