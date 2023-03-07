// MyGetProcAddress.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include <string>
#include <iostream>
#include "MyGetProcAddress.h"
using namespace std;
typedef int (_stdcall * MyAddType)(int a, int b);
typedef int (_stdcall * MyMsgBox)(DWORD, const char *, const char *, DWORD);
FARPROC MyGetProcAddress(HMODULE hModule, LPSTR lpProcName)
{
    IMAGE_DOS_HEADER dosHdr = {};
    IMAGE_NT_HEADERS ntHdr = {};
    IMAGE_DATA_DIRECTORY* pDataEntry = nullptr;
    IMAGE_EXPORT_DIRECTORY* pExp = nullptr;
    RtlCopyMemory(&dosHdr, hModule, sizeof(IMAGE_DOS_HEADER));
    RtlCopyMemory(&ntHdr, (void*)(dosHdr.e_lfanew + (DWORD)hModule), sizeof(IMAGE_NT_HEADERS));
    pDataEntry = (IMAGE_DATA_DIRECTORY*) & ntHdr.OptionalHeader.DataDirectory;
    pExp = (IMAGE_EXPORT_DIRECTORY*)(pDataEntry->VirtualAddress + (DWORD)hModule);
    DWORD ordinal = -1;

    IMAGE_EXPORT_DIRECTORY* currExp = pExp;
    if ((DWORD)lpProcName & 0xffff0000)
    {
        DWORD* nameAddr = (DWORD*)(currExp->AddressOfNames + (DWORD)hModule);
        
        int i = 0;
        while (i < currExp->NumberOfNames)
        {
            DWORD* currAddr = nameAddr + i;
            char* pName = (char*)(*currAddr + (DWORD)hModule);
            int nRet = strcmp(pName, lpProcName);
            if (nRet == 0)
            {
                DWORD tempAddr = currExp->AddressOfNameOrdinals + (DWORD)hModule;
                ordinal = *((WORD*)tempAddr + i);
                break;
            }
            i++;
        }
    }
    else
    {
        ordinal = (DWORD)lpProcName - currExp->Base;
    }
    DWORD* funcAddrList = (DWORD*)(currExp->AddressOfFunctions + (DWORD)hModule);
    DWORD currFuncAddr = *(funcAddrList + ordinal);
    if (currFuncAddr >= pDataEntry->VirtualAddress && currFuncAddr <= (pDataEntry->VirtualAddress + pDataEntry->Size))
    {
        string sOtherDllFunc = (char*)(currFuncAddr + (DWORD)hModule);
        int nPointPos = sOtherDllFunc.find_first_of('.');
        string sDllName = sOtherDllFunc.substr(0, nPointPos);
        string sFuncName = sOtherDllFunc.substr(nPointPos + 1);
        HMODULE hDll = GetModuleHandle(sDllName.c_str());
        return MyGetProcAddress(hDll, (LPSTR)sFuncName.c_str());
    }
    else
    {
        return (FARPROC) (currFuncAddr + (DWORD)hModule);
    }
  
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    HMODULE hModule = LoadLibrary("Test.Dll");
    MyAddType pMyAdd = (MyAddType)MyGetProcAddress(hModule, (LPSTR)"MyAdd");
    pMyAdd(1, 2);
    MyMsgBox pMyMsg = (MyMsgBox)MyGetProcAddress(hModule, (LPSTR)"MyMsgBox");
    pMyMsg(NULL, "TEST MSG", "TEST", MB_OK);
    return 0;
}
