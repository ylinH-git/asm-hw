// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "Plugin.h"
#include <windows.h>
typedef signed int(__stdcall* fnRtlGetVersion)(OSVERSIONINFOW&);
void EnableFilterExpBreak();

int ODBG_Plugindata(char* shortname)
{
    strcpy(shortname, "excptplugin");
    return PLUGIN_VERSION;
}

int ODBG_Plugininit(int ollydbgversion, HWND hw, ulong* features)
{
    return 0;
}

int ODBG_Pluginmenu(int origin, char data[4096], void* item)
{
    if (origin == PM_MAIN)
    {
        strcpy(data, "0 启用筛选器断点");
    }

    return 1;
}

void ODBG_Pluginaction(int origin, int action, void* item)
{
    EnableFilterExpBreak();
    MessageBox(NULL, TEXT("补丁已安装"), NULL, MB_OK);
}

void EnableFilterExpBreak()
{
    //找到UnhandledExceptionFilter + 0xcb
    HMODULE hKernelbase = GetModuleHandle("kernelbase");
    if (hKernelbase != 0) 
    {
        LPBYTE pUEF = (LPBYTE)GetProcAddress(hKernelbase, "UnhandledExceptionFilter");
        uchar* curPoint = pUEF;
        OSVERSIONINFOW info = {};
        auto RtlGetVersion = (fnRtlGetVersion)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");
        if (RtlGetVersion != 0 && RtlGetVersion(info) == 0)
        {
            // 虚拟机挂了，拿win11做示例
            if (info.dwMajorVersion >= 10) {
                uchar signatures[6] = { 0xFF, 0x15, 0x48,0x02,0x97, 0x77 };
                uchar signatures2[4] = { 0x85, 0xc0, 0x0f, 0x85 };
                while (memcmp(curPoint, signatures, 6) != 0) {
                    curPoint++;
                }
                while (memcmp(curPoint, signatures2, 4) != 0)
                {
                    curPoint++;
                }
                LPBYTE pFixAddr = curPoint + 2;
                if (*pFixAddr == 0x0f && *(pFixAddr + 1) == 0x85)
                {
                    //打补丁，nop 掉6个字节
                    char buf[6] = { 0x90,0x90,0x90,0x90,0x90,0x90 };
                    Writememory(buf, (ulong)pFixAddr, sizeof(buf), MM_RESTORE | MM_DELANAL | MM_SILENT);
                }
            }
        }
    }
}

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

