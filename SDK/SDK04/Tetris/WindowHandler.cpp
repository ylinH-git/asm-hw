#include "framework.h"
void ShowError()
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION);
	// Free the buffer.
	LocalFree(lpMsgBuf);
}

ATOM CreateWindowHandler(
	_In_ HINSTANCE hInstance,
	LPCTSTR className,
	LPCTSTR windowTitle
)
{
	HWND hWnd = CreateWindow(
		className,            // 窗口类名
		windowTitle,          // 窗口标题
		WS_OVERLAPPEDWINDOW,  // 窗口风格，重叠窗口（主窗口）
		110, 110, 800, 600,   // 坐标和尺寸
		NULL,                 // 没有父窗口
		NULL,                 // 没有菜单
		hInstance,            // 实例句柄
		NULL                  // 没有参数
	);

	if (hWnd == NULL)
	{
		ShowError();
		return 0;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}