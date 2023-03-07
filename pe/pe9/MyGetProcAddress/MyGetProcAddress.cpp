// MyGetProcAddress.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include <string>
#include <iostream>
#include "MyGetProcAddress.h"

using namespace std;

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
            nameAddr += i;
            char* pName = (char*)(nameAddr + (DWORD)hModule);
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

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    return 0;
}
