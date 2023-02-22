// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#pragma comment(lib, "capstone.lib")
#include "include/capstone/capstone.h"


extern "C" __declspec(dllexport)
size_t __stdcall DisasmLine(uint8_t * code, size_t nCodeSize, uint32_t nAddr,
    char* szAsm)
{
    int ret = 0;
    csh handle;

    auto nErr = cs_open(CS_ARCH_X86, CS_MODE_32, &handle);
    if (nErr != CS_ERR_OK)
    {
        return ret;
    }

    cs_insn* insn;
    size_t count = cs_disasm(handle, code, nCodeSize, nAddr, 1, &insn);
    if (count == 0)
    {
        return ret;
    }

    strcpy(szAsm, insn->mnemonic);
    strcat(szAsm, " ");
    strcat(szAsm, insn->op_str);
    ret = insn->size;
    cs_free(insn, count);
    cs_close(&handle);

    return ret;
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

