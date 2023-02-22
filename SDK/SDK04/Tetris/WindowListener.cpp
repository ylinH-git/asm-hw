#include "framework.h"
#include "CGame.h"
#include "WindowListener.h"

CGame hGame;
int nTimer = 1;

void CommandHandler(
	HWND hwnd,
	WPARAM wParam,
	LPARAM lParam) 
{
	switch (wParam)
	{
	case RESTART:
		hGame.Restart();
		break;
	case PAUSE:
		hGame.SetIsExceed(false);
		break;
	case START:
		hGame.SetIsExceed(true);
		break;
	}
}
LRESULT CALLBACK WndPoc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		SetTimer(hwnd, nTimer, (UINT)(300), (TIMERPROC)NULL);
		break;
	}
	case WM_PAINT:
	{
		hGame.MoveDown();
		hGame.ShowGame(hwnd, wParam, lParam);
		break;
	}
	case WM_CLOSE: // 点击关闭按钮
	{
		KillTimer(hwnd, nTimer);
		PostQuitMessage(0); // 退出消息循环
		break;
	}
	case WM_TIMER:
	{
		switch (wParam) {
		case 1:
		{
			InvalidateRect(hwnd, NULL, true);
			UpdateWindow(hwnd);
			break;
		}
		}

		OutputDebugString(TEXT("WM_TIMER"));
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'A':
			hGame.MoveLeft();
			break;
		case 'S':
			hGame.MoveDown();
			break;
		case 'W':
			hGame.MoveUp();
			break;
		case 'D':
			hGame.MoveRight();
			break;
		}
		break;
	}
	case WM_COMMAND:
	{
		CommandHandler(hwnd, wParam, lParam);
		break;
	}
	default:
		// 使用系统提供的默认处理方法
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}