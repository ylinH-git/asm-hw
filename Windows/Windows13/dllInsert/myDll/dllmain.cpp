// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#define OPENMESSAGE 40001
#define DESTORYMENU 40002
#define MAINMENU 40003

LRESULT CALLBACK MyWindowProc(
	HWND hwnd,      // handle to window 窗口句柄
	UINT uMsg,      // message identifier	消息标识
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
);

WNDPROC g_pfnOldProc = NULL;
HMENU g_popMenu = NULL;
HMODULE g_hModule = NULL;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        g_popMenu = CreatePopupMenu();
        g_hModule = hModule;
        HWND hWin = FindWindow(NULL, "扫雷");
        HMENU hWinMenu = GetMenu(hWin);
        AppendMenu(g_popMenu,  MF_STRING, OPENMESSAGE, "打开弹窗");
        AppendMenu(g_popMenu,  MF_STRING, DESTORYMENU, "销毁菜单");
        AppendMenu(hWinMenu, MF_POPUP, (UINT)g_popMenu, "注入菜单");
		g_pfnOldProc = (WNDPROC)SetWindowLongPtrA(hWin, GWLP_WNDPROC, (LONG_PTR)MyWindowProc);
        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


DWORD WINAPI ThreadProc(
    _In_ LPVOID lpParameter
)
{
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;

}

//窗口过程函数的实现(消息处理函数)
LRESULT CALLBACK MyWindowProc(
	HWND hwnd,      // handle to window 窗口句柄
	UINT uMsg,      // message identifier	消息标识
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
)
{

	switch (uMsg)
	{
	case WM_COMMAND: //相应菜单消息
		switch (LOWORD(wParam))
		{
		case OPENMESSAGE: MessageBox(hwnd,"创建弹窗", NULL, MB_OK); return 0;
		case DESTORYMENU: 
        {
            HMENU hMenu = GetMenu(hwnd);
            DeleteMenu(hMenu, 2, MF_BYPOSITION);
            DrawMenuBar(hwnd);
            // 需要先把劫持的过程函数释放才可以下一步
            SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)g_pfnOldProc);
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, NULL);- 
            CloseHandle(hThread);
#if 0
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            CreateProcess(NULL,   // No module name (use command line)
                (LPSTR)"..\\Debug\\removeDll.exe",        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory 
                &si,            // Pointer to STARTUPINFO structure
                &p
                i);
#endif // 0
        }
        default: break;
		}
    default: break;
	}

    return CallWindowProc(g_pfnOldProc, hwnd, uMsg, wParam, lParam);

}
