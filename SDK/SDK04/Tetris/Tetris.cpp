// Tetris.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowHandler.h"
#include "WindowListener.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	HINSTANCE hIns = hInstance;
	WNDCLASS wc = {};
	g_zhuanIco = LoadIcon(hInstance, MAKEINTRESOURCE(ZHUAN_BLUE));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndPoc;
	wc.hInstance = hInstance;
	wc.hIcon = g_zhuanIco;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(GAME_MENU); // 菜单名
	wc.lpszClassName = TEXT("Tetris"); // 窗口类名


	ATOM nRet = RegisterClass(&wc);
	if (nRet == 0)
	{
		MessageBox(NULL, TEXT("注册失败"), TEXT("提示"), MB_OK);
		return 0;
	}

	CreateWindowHandler(
		hInstance,
		TEXT("Tetris"),
		TEXT("Tetris")
	);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
}