// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "../MySpxx/resource.h"
#pragma data_seg("HookShareData") //数据段开始
__declspec(dllexport) DWORD g_threadId = NULL;
__declspec(dllexport) HWND g_winHwnd  = NULL;
__declspec(dllexport) HWND g_curHwnd = NULL;
#pragma data_seg() //数据段的结束

//设置数据段为可共享
#pragma comment(linker, "/SECTION:HookShareData,RWS")

HHOOK g_hHook = NULL;
HMODULE g_hModule = NULL;

LRESULT CALLBACK GetMsgProc(int code,
	WPARAM wParam,
	LPARAM lParam
)
{
	if (code < 0)
	{
		return CallNextHookEx(g_hHook, code, wParam, lParam);
	}

	MSG msg = *(MSG*)lParam;

	// 获取全部窗口的信息后再判断信息是否是命令且hwnd是不是当前窗口的hwnd，一直都不会满足
	if (msg.message == WM_COMMAND && msg.hwnd == g_curHwnd)
	{
		if (g_winHwnd)
		{
			HWND hEdit = GetDlgItem(g_winHwnd, EDT_HOOKRESULT);
			int nLen = GetWindowTextLength(hEdit);
			char* cEditText = new char[nLen + 1];
			GetDlgItemText(g_winHwnd, EDT_HOOKRESULT, cEditText, nLen + 1);

			//处理
			TCHAR szBuf[MAXBYTE] = {};
			wsprintf(szBuf, TEXT("[hook] WM_COMMAND %d"), wParam);
			char* cEditTextNew = new char[nLen + MAXBYTE + 1];
			wsprintf(cEditTextNew, TEXT("%s\r\n%s"), cEditText, szBuf);
			SetDlgItemText(g_winHwnd, EDT_HOOKRESULT, cEditTextNew);
			delete[] cEditText;
			delete[] cEditTextNew;
		}
	}

	return CallNextHookEx(g_hHook, code, wParam, lParam);
}
__declspec(dllexport) void InstallHook()
{
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE,
		GetMsgProc, //键盘钩子回调函数
		g_hModule, //全局钩子填写钩子回调函数所在的模块的句柄
		// g_threadId // 无法响应hook回调
		0 //所有窗口
	);
	if (g_hHook == NULL)
	{
		MessageBox(NULL, TEXT("钩子安装失败"), NULL, MB_OK);
	}
}


__declspec(dllexport)void UnInstallHook()
{
	// TODO: 在此添加控件通知处理程序代码
	::UnhookWindowsHookEx(g_hHook);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

