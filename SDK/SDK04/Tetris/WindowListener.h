#pragma once
#include "framework.h"
#include "CGame.h"
LRESULT CALLBACK WndPoc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

extern CGame hGame;
extern int nTimer;

void CommandHandler(
	HWND hwnd,
	WPARAM wParam,
	LPARAM lParam);