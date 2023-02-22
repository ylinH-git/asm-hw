// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#define OPENHELPER 40001
#define CLOSEHELPER 40002
#define DESTORYHELPER 40003

void CloseHelper();

void DestoryProcess();

LRESULT CALLBACK MyWindowProc(
	HWND hwnd,      // handle to window 窗口句柄
	UINT uMsg,      // message identifier	消息标识
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
);

WNDPROC g_pfnOldProc = NULL;
HMENU g_popMenu = NULL;
HMODULE g_hModule = NULL;
HWND g_hHelperWin = NULL;
HANDLE g_hProc = NULL;
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
        HWND hWin = FindWindow(NULL, "摿孭");
        HMENU hWinMenu = CreateMenu();
        AppendMenu(g_popMenu,  MF_STRING, OPENHELPER, "打开辅助");
        AppendMenu(g_popMenu,  MF_STRING, CLOSEHELPER, "关闭辅助");
        AppendMenu(g_popMenu, MF_STRING, DESTORYHELPER, "卸载辅助");
        AppendMenu(hWinMenu, MF_POPUP, (UINT)g_popMenu, "辅助菜单");
        SetMenu(hWin, hWinMenu);
        //DrawMenuBar(hWin);
		g_pfnOldProc = (WNDPROC)SetWindowLongPtrA(hWin, GWLP_WNDPROC, (LONG_PTR)MyWindowProc);
        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DestoryProcess();
        break;
    }
    return TRUE;
}


DWORD WINAPI ThreadProc(
    _In_ LPVOID lpParameter
)
{
    Sleep(10);
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;

}

void CloseHelper()
{
    if (g_hHelperWin != NULL)
    {
        ShowWindow(g_hHelperWin, SW_HIDE);
    }
}
void DestoryProcess()
{
    SendMessage(g_hHelperWin, WM_CLOSE, NULL, NULL);
    g_hHelperWin = NULL;
    TerminateProcess(g_hProc, 0);
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
		case OPENHELPER: 
        {
            if (g_hHelperWin == NULL)
            {
                STARTUPINFO si;
                PROCESS_INFORMATION pi;

                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));

                BOOL nRet = CreateProcess(
                    NULL,
                    (LPSTR)R"(GameHelper.exe)",
                    NULL, NULL, FALSE,
                    0,
                    NULL,
                    NULL,
                    &si,
                    &pi
                );
                if (nRet == NULL)
                {
                    MessageBox(hwnd, "打开辅助工具失败", NULL, MB_OK);
                }
                else
                {
                    Sleep(100);
                    g_hHelperWin = FindWindow(NULL, "辅助工具");
                    g_hProc = pi.hProcess;
                    CloseHandle(pi.hThread);
                }
            }
            else {
                ShowWindow(g_hHelperWin, SW_SHOW);
            }
            return 0;
        }
        case CLOSEHELPER:
        {
            CloseHelper();
            return 0;
        }
		case DESTORYHELPER:
        {
            HMENU hMenu = GetMenu(hwnd);
            DeleteMenu(hMenu, 0, MF_BYPOSITION);
            DrawMenuBar(hwnd);
            // 需要先把劫持的过程函数释放才可以下一步
            SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)g_pfnOldProc);
            DestoryProcess();
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, NULL);
            if (hThread)
            {
                CloseHandle(hThread);
            }
        }
        default: break;
		}
    default: break;
	}

    return CallWindowProc(g_pfnOldProc, hwnd, uMsg, wParam, lParam);

}
