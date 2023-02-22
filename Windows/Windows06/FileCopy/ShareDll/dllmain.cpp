// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"


#pragma data_seg("FileCopyShareData") //数据段开始
__declspec(dllexport) int g_progress = 0;
__declspec(dllexport) char g_sourcePath[MAX_PATH] = {};
__declspec(dllexport) char g_destPath[MAX_PATH] = {};
#pragma data_seg() //数据段的结束

//设置数据段为可共享
#pragma comment(linker, "/SECTION:FileCopyShareData,RWS")


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

