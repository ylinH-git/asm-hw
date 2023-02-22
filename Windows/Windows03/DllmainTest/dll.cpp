
#include <iostream>
#include <windows.h>
typedef BOOL(WINAPI* __scrt_dllmain_type)(HINSTANCE, DWORD, LPVOID);
BOOL __stdcall TestDllMain(HINSTANCE hinstDLL,
    DWORD fdwReason,    
    LPVOID lpvReserved)
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        std::cout << "1234" << std::endl;
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:

        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
extern __scrt_dllmain_type const _pRawDllMain = TestDllMain;
extern __scrt_dllmain_type const _pDefaultRawDllMain = TestDllMain;

__declspec(dllexport) void Test() {
	std::cout << "test" << std::endl;
}