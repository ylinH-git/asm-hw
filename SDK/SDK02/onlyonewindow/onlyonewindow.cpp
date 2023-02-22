// onlyonewindow.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "onlyonewindow.h"
#include <vector>
#include <iostream>
#include <string>

std::vector<HWND> vHwnds = {};
HINSTANCE hIns = nullptr;
LRESULT CALLBACK Cr44WndProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			if (hIns != nullptr)
			{
				std::wstring wTitle = TEXT("窗口");
				auto nextIdx = vHwnds.size() + 1;
				wTitle.append(std::to_wstring(nextIdx));
				CreateWindowHandler(
					hIns,
					TEXT("窗口"),
					wTitle.c_str()
				);
			}
			return 0;
		}
		case WM_CLOSE: // 点击关闭按钮
			if (vHwnds.size() > 1)
			{
				bool isDestory = DestroyWindow(hwnd);
				if (isDestory) 
				{
					std::vector<HWND>::iterator it;
					for (it = vHwnds.begin(); it != vHwnds.end(); ++it)
					{
						if (*it == hwnd)
						{
							break;
						}
					}
					vHwnds.erase(it);
				}
			}
			else
			{
				PostQuitMessage(0); // 退出消息循环
			}
			return 0;
		default:
			break;
	}
	// 使用系统提供的默认处理方法
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	hIns = hInstance;
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 窗口过程函数（回调函数）
	wc.lpfnWndProc = Cr44WndProc;
	//实例句柄
	wc.hInstance = hInstance;
	wc.hIcon = NULL; // 图标
	wc.hCursor = NULL; // 光标
	// 背景色
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
	// 从队列中抽取所有窗口的所有信息
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// 一般不会手动去调用回调函数
		// Cr44WndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam); 

		// 派发消息，根据窗口句柄调用对应窗口的过程函数
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
		110, 110, 120, 120,   // 坐标和尺寸
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

	vHwnds.push_back(hWnd);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}