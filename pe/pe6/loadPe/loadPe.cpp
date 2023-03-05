// loadPe.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "loadPe.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    HANDLE hFile;
    HANDLE hMap;
    LPVOID pAddr;
    IMAGE_DOS_HEADER* pDosHdr;
    IMAGE_NT_HEADERS* pNtHdr;
    IMAGE_SECTION_HEADER* pSecHdr;
    DWORD dwNumOfSecs;
    DWORD dwOldProc;
    IMAGE_IMPORT_DESCRIPTOR* pImp;
    IMAGE_IMPORT_DESCRIPTOR impEmpty = {};
    HMODULE hDll;
    LPVOID pINT;
    LPVOID pIAT;
    DWORD dwOep;

    RtlZeroMemory(&impEmpty, sizeof(IMAGE_IMPORT_DESCRIPTOR));

    hFile = CreateFile("test.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    pAddr = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    // 解析格式
    pDosHdr = (IMAGE_DOS_HEADER*)pAddr;
    pNtHdr = (IMAGE_NT_HEADERS*)((DWORD)pDosHdr->e_lfanew + (DWORD)pAddr);
    pSecHdr = (IMAGE_SECTION_HEADER*)((DWORD)&pNtHdr->OptionalHeader + (DWORD)pNtHdr->FileHeader.SizeOfOptionalHeader);
    dwNumOfSecs = pNtHdr->FileHeader.NumberOfSections;
    dwOep = (DWORD)((DWORD)pNtHdr->OptionalHeader.AddressOfEntryPoint + (DWORD)hInstance);
    // 修改内存属性
    VirtualProtect(hInstance, pNtHdr->OptionalHeader.SizeOfImage, PAGE_EXECUTE_READWRITE, &dwOldProc);

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
        currSecHdr+=1;
        i++;
    }

    IMAGE_DATA_DIRECTORY* pDataDir = (IMAGE_DATA_DIRECTORY*)&pNtHdr->OptionalHeader.DataDirectory;
    pDataDir += 1;
    pImp = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)pDataDir->VirtualAddress + (DWORD)hInstance);
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

        char* pName =(char*)(currImp->Name + (DWORD)hInstance);
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
            pIAT = (DWORD*) pIAT + 1;
            pINT = (DWORD*) pINT + 1;
            dwINT = *(DWORD*)pINT;
        }

        currImp += 1;
    }
    ((FARPROC)dwOep)();
    return 0;
}
