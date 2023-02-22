// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

bool map[5][9] = { 0 };
typedef void(*PlanFn)(DWORD, DWORD, DWORD, DWORD, DWORD);
DWORD WINAPI ThreadProc(
    _In_ LPVOID lpParameter
)
{
    bool isTrue = false;
    while (1)
    {
        HMODULE hBase = GetModuleHandle("PlantsVsZombies0.exe");
        DWORD hGameBase = *(DWORD*)((DWORD)hBase + 0x2a9ec0);
        DWORD hEnvBase = *(DWORD*)((DWORD)hGameBase + 0x768);
        DWORD hPlantHandle = 0x40D120;
        if (hEnvBase == NULL)
        {
            continue;
        }
        DWORD hLastZombie = *(DWORD*)(hEnvBase + 0x90);
        DWORD hFisrtZombie = hLastZombie + 0x15c * 8;
        DWORD currZombie = hFisrtZombie;
        while (currZombie && currZombie != hLastZombie)
        {
            DWORD zombieRow = *(DWORD*)(currZombie + 0x1c);
            DWORD zombieType = *(DWORD*)(currZombie + 0x24);
            float zombiePos = *(float*)(currZombie + 0x2c);
            DWORD zombieHp = *(DWORD*)(currZombie + 0xc8);
            int col = 0;
            int row = zombieRow;
            while (map[row][col]) {
                
                if (col > 8)
                {
                    break;
                }
                col++;
            }
            if (zombiePos < 750 && zombieHp > 0 && col < 9 && row < 5)
            {
                map[row][col] = true;
                DWORD plantType = 0;
                if (col < 2)
                {
                    plantType = 1;
                }
                if (col > 7)
                {  
                    plantType = 3;
                }
                _asm
                {
                    push -1
                    push plantType
                    push col
                    push hEnvBase
                    mov eax, row
                    call hPlantHandle
                }
            }
            currZombie -= 0x15c;
        }
    }
    return 0;

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, NULL);
        if (hThread != 0)
        {
            CloseHandle(hThread);
        }
        break;
    }  
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

