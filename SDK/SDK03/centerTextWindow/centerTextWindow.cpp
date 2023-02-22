// centerTextWindow.cpp : ����Ӧ�ó������ڵ㡣
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
		case WM_CLOSE: // ����رհ�ť
		{
			PostQuitMessage(0); // �˳���Ϣѭ��
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
			// ʹ��ϵͳ�ṩ��Ĭ�ϴ�����
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
	wc.lpszMenuName = NULL; // �˵���
	wc.lpszClassName = TEXT("����"); // ��������


	ATOM nRet = RegisterClass(&wc);
	if (nRet == 0)
	{
		MessageBox(NULL, TEXT("ע��ʧ��"), TEXT("��ʾ"), MB_OK);
		return 0;
	}

	CreateWindowHandler(
		hInstance,
		TEXT("����"),
		TEXT("����1")
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
		className,            // ��������
		windowTitle,          // ���ڱ���
		WS_OVERLAPPEDWINDOW,  // ���ڷ���ص����ڣ������ڣ�
		110, 110, 800, 600,   // ����ͳߴ�
		NULL,                 // û�и�����
		NULL,                 // û�в˵�
		hInstance,            // ʵ�����
		NULL                  // û�в���
	);

	if (hWnd == NULL)
	{
		ShowError();
		return 0;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}