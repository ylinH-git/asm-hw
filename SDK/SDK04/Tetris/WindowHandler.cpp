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