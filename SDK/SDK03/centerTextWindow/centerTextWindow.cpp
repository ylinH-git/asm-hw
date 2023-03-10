// centerTextWindow.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include <iostream>
#include <string>

void ShowError();
ATOM CreateWindowHandler(
	_In_ HINSTANCE hInstance,
	LPCTSTR className,
	LPCTSTR windowTitle
);
HINSTANCE hIns;
int hasInitTimer = 0;
RECT rc;
int nCurrentX = 0;
int nCurrentY = 0;
RECT winRect;
int nTimer = 1;
LRESULT CALLBACK WndPoc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	HDC hdc;
	PAINTSTRUCT ps = {};
	time_t now;
	switch (uMsg)
	{
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			now = time(0);
			char tStr[MAXBYTE] = {};
			ctime_s(tStr, MAXBYTE, &now);
			GetClientRect(hwnd, &winRect);
			FillRect(hdc, &winRect, (HBRUSH)(COLOR_HIGHLIGHT + 1));
			rc.left = winRect.left + nCurrentX;
			rc.right = winRect.right + nCurrentX;
			rc.top = winRect.top + nCurrentY;
			rc.bottom = winRect.bottom + nCurrentY;
			DrawText(hdc, tStr, strlen(tStr), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			EndPaint(hwnd, &ps);
			if (hasInitTimer == 0)
			{
				GetClientRect(hwnd, &rc);
				hasInitTimer = SetTimer(hwnd, nTimer, 1000, (TIMERPROC)NULL);
			}
			OutputDebugString(TEXT("WM_PAINT"));
			break;
		}
		case WM_RBUTTONDOWN:
		{
			KillTimer(hwnd, nTimer);
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case 'W':
				nCurrentY -= 5;
				break;
			case 'S':
				nCurrentY += 5;
				break;
			case 'A':
				nCurrentX -= 5;
				break;
			case 'D':
				nCurrentX += 5;
				break;
			}
			break;
		}
		case WM_CLOSE: // 点击关闭按钮
		{
			PostQuitMessage(0); // 退出消息循环
			break;
		}
		case WM_TIMER:
		{
			switch (wParam) {
				case 1:
				{			
					InvalidateRect(hwnd, NULL, false);
					break;
				}
			}

			OutputDebugString(TEXT("WM_TIMER"));
			break;
		}
		default:
			// 使用系统提供的默认处理方法
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}	
	return 0;
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	hIns = hInstance;
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndPoc;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_HIGHLIGHT + 1);
	wc.lpszMenuName = NULL; // 菜单名
	wc.lpszClassName = TEXT("窗口"); // 窗口类名


	ATOM nRet = RegisterClass(&wc);
	if (nRet == 0)
	{
		MessageBox(NULL, TEXT("注册失败"), TEXT("提示"), MB_OK);
		return 0;
	}

	CreateWindowHandler(
		hInstance,
		TEXT("窗口"),
		TEXT("窗口1")
	);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
}


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