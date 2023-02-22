// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#pragma comment(linker, "/EXPORT:d2ld=ldconvert_old.d2ld")
#pragma comment(linker, "/EXPORT:ld2d=ldconvert_old.ld2d")
#pragma comment(linker, "/EXPORT:ld2str=ldconvert_old.ld2str")
#pragma comment(linker, "/EXPORT:str2ld=ldconvert_old.str2ld")


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, "你被劫持了", NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

