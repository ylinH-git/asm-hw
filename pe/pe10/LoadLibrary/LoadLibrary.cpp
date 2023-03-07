// LoadLibrary.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "LoadLibrary.h"
#include <string>
#include <iostream>
using namespace std;

typedef BOOL(APIENTRY* DllMainFn)(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    );

typedef int(_stdcall* MyAddType)(int a, int b);

FARPROC MyGetProcAddress(HMODULE hModule, LPSTR lpProcName)
{
    IMAGE_DOS_HEADER dosHdr = {};
    IMAGE_NT_HEADERS ntHdr = {};
    IMAGE_DATA_DIRECTORY* pDataEntry = nullptr;
    IMAGE_EXPORT_DIRECTORY* pExp = nullptr;
    RtlCopyMemory(&dosHdr, hModule, sizeof(IMAGE_DOS_HEADER));
    RtlCopyMemory(&ntHdr, (void*)(dosHdr.e_lfanew + (DWORD)hModule), sizeof(IMAGE_NT_HEADERS));
    pDataEntry = (IMAGE_DATA_DIRECTORY*)&ntHdr.OptionalHeader.DataDirectory;
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
        return (FARPROC)(currFuncAddr + (DWORD)hModule);
    }

}

HMODULE MyLoadLibrary(LPSTR lpszName)
{
    HANDLE hFile;
    HANDLE hMap;
    LPVOID pAddr = nullptr;
    IMAGE_DOS_HEADER* pDosHdr = nullptr;
    IMAGE_NT_HEADERS* pNtHdr = nullptr;
    IMAGE_SECTION_HEADER* pSecHdr = nullptr;
    DWORD dwNumOfSecs = 0;
    DWORD dwOldProc = 0;
    IMAGE_IMPORT_DESCRIPTOR* pImp = nullptr;
    IMAGE_IMPORT_DESCRIPTOR impEmpty = {};
    IMAGE_BASE_RELOCATION* pReloc = nullptr;
    HMODULE hDll;
    LPVOID pINT;
    LPVOID pIAT;
    DWORD dwOep = 0;
    DWORD dwSizeOfImg = 0;
    HMODULE hInstance = nullptr;
    DWORD dwSizeOfReloc = 0;
    DWORD dwCountOfReloc = 0;
    DWORD dwOffset = 0;

    RtlZeroMemory(&impEmpty, sizeof(IMAGE_IMPORT_DESCRIPTOR));

    hFile = CreateFile(lpszName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMap) {
        pAddr = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    }

    // 解析格式
    if (pAddr) {
        pDosHdr = (IMAGE_DOS_HEADER*)pAddr;
    }

    if (pDosHdr) {
        pNtHdr = (IMAGE_NT_HEADERS*)((DWORD)pDosHdr->e_lfanew + (DWORD)pAddr);
    }
    if (pNtHdr)
    {
        dwSizeOfImg = pNtHdr->OptionalHeader.SizeOfImage;


        pSecHdr = (IMAGE_SECTION_HEADER*)((DWORD)&pNtHdr->OptionalHeader + (DWORD)pNtHdr->FileHeader.SizeOfOptionalHeader);
        dwNumOfSecs = pNtHdr->FileHeader.NumberOfSections;

        // 申请内存
        hInstance = (HMODULE)VirtualAlloc(NULL, dwSizeOfImg, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        dwOep = (DWORD)((DWORD)pNtHdr->OptionalHeader.AddressOfEntryPoint + (DWORD)hInstance);

        // 计算新地址和旧基址的偏移
        dwOffset = (DWORD)hInstance - pNtHdr->OptionalHeader.ImageBase;
        // 拷贝PE头
        RtlCopyMemory(hInstance, pAddr, pNtHdr->OptionalHeader.SizeOfHeaders);

        // 拷贝节数据
        int i = 0;
        IMAGE_SECTION_HEADER* currSecHdr = pSecHdr;
        while (i < dwNumOfSecs)
        {
            // 大小
            DWORD dataSize = currSecHdr->SizeOfRawData;
            // 目标
            DWORD va = (DWORD)currSecHdr->VirtualAddress + (DWORD)hInstance;
            // 源
            DWORD pRawData = (DWORD)currSecHdr->PointerToRawData + (DWORD)pAddr;
            RtlCopyMemory((void*)va, (void*)pRawData, dataSize);
            currSecHdr += 1;
            i++;
        }

        IMAGE_DATA_DIRECTORY* pDataDir = (IMAGE_DATA_DIRECTORY*)&pNtHdr->OptionalHeader.DataDirectory;
        pDataDir += 1;
        pImp = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)pDataDir->VirtualAddress + (DWORD)hInstance);

        pDataDir += 4;
        pReloc = (IMAGE_BASE_RELOCATION*)((DWORD)pDataDir->VirtualAddress + (DWORD)hInstance);
        if (pReloc) {
            dwSizeOfReloc = pReloc->SizeOfBlock;
        }
        // 处理导入表
        IMAGE_IMPORT_DESCRIPTOR* currImp = pImp;
        while (true)
        {
            int nRet = memcmp(currImp, &impEmpty, sizeof(IMAGE_IMPORT_DESCRIPTOR));
            if (nRet == 0)
            {
                break;
            }

            if (currImp->Name == NULL)
            {
                break;
            }

            char* pName = (char*)(currImp->Name + (DWORD)hInstance);
            hDll = LoadLibrary(pName);

            // 判断FT
            if (currImp->FirstThunk == NULL)
            {
                break;
            }

            // 获取INT
            DWORD pINTTemp = currImp->FirstThunk;
            if (currImp->OriginalFirstThunk != NULL)
            {
                pINTTemp = currImp->OriginalFirstThunk;
            }
            pINTTemp = pINTTemp + (DWORD)hInstance;
            pINT = (LPVOID)pINTTemp;

            // 获取IAT
            DWORD pIATTemp = currImp->FirstThunk;
            pIATTemp = pIATTemp + (DWORD)hInstance;
            pIAT = (LPVOID)pIATTemp;


            DWORD dwINT = *(DWORD*)pINT;
            while (dwINT != 0)
            {
                if (hDll) {
                    if (dwINT & 0x80000000)
                    {
                        pIAT = GetProcAddress(hDll, (LPCSTR)((dwINT << 16) >> 16));
                    }
                    else
                    {
                        char* pFName = (char*)(dwINT + (DWORD)hInstance + 2);
                        FARPROC pFunc = GetProcAddress(hDll, pFName);
                        RtlCopyMemory(pIAT, &pFunc, sizeof(DWORD));
                    }
                }
               
                pIAT = (DWORD*)pIAT + 1;
                pINT = (DWORD*)pINT + 1;
                dwINT = *(DWORD*)pINT;
            }

            currImp += 1;
        }

        // 处理重定位表
        i = 0;
        IMAGE_BASE_RELOCATION* currBase = pReloc;
        while (i < dwSizeOfReloc) {
            WORD* addrList = (WORD*)((DWORD*)pReloc + 2);
            dwCountOfReloc = (pReloc->SizeOfBlock - 8) / sizeof(WORD);
            for (int j = 0; j < dwCountOfReloc; j++)
            {
                WORD currAddr = addrList[j];
                if (currAddr & 0xf000)
                {
                    WORD realAddr = currAddr & 0x0fff;
                    DWORD realVa = realAddr + pReloc->VirtualAddress + (DWORD)hInstance;
                    DWORD newAddr = *(DWORD*)realVa + dwOffset;
                    RtlCopyMemory((DWORD*)realVa, &newAddr, sizeof(DWORD));
                }
            }
            i += pReloc->SizeOfBlock;
            currBase = (IMAGE_BASE_RELOCATION*)((DWORD)currBase + pReloc->SizeOfBlock);
        }
    }

    if (dwOep) {
        DllMainFn dllMain = (DllMainFn)dwOep;
        dllMain(hInstance, DLL_PROCESS_ATTACH, 0);
    }
  
    return hInstance;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    HMODULE hDll = MyLoadLibrary((LPSTR)"Test.dll");
    MyAddType MyAdd = (MyAddType)MyGetProcAddress(hDll, (LPSTR)"MyAdd");
    MyAdd(1, 2);
    return 0;
}