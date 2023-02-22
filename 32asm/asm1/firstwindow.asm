.386
.model flat, stdcall
option casemap:none

include windows.inc
include kernel32.inc
include user32.inc

includelib Kernel32.lib
includelib User32.lib

.data
    g_szWindowClassName db "first masm32 window class", 0
    g_szWindowTitle db "第一个窗口", 0
    g_szTime db 255 dup(?), 0
    g_winRect RECT<?>
    g_hasInitTimer db 0
    g_nTimer db 1
    g_ps PAINTSTRUCT<?>
    g_st SYSTEMTIME<?>
.code

WndProc proc hWnd:HWND, message:UINT, wParam:WPARAM, lParam:LPARAM
    local @hdc: HDC
    local @nLen:dword

    invoke RtlZeroMemory, offset g_st, size g_st
    invoke RtlZeroMemory, offset g_ps, size g_ps
    mov @hdc, 0

    .IF message == WM_DESTROY
        invoke KillTimer, hWnd, 1
        invoke PostQuitMessage, 0
    .ELSEIF message == WM_PAINT
        invoke BeginPaint, hWnd, offset g_ps
        mov @hdc,eax
        invoke GetLocalTime, offset g_st
        invoke GetTimeFormat, LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT,offset g_st, NULL, offset g_szTime, 255
        invoke GetClientRect, hWnd, offset g_winRect

        invoke lstrlen,offset g_szTime
        mov @nLen,eax
        invoke FillRect, @hdc,  offset g_winRect, COLOR_HIGHLIGHT + 1
        invoke DrawText, @hdc, offset g_szTime, @nLen, offset g_winRect, DT_CENTER or DT_VCENTER or DT_SINGLELINE
        invoke EndPaint, hWnd, offset g_ps
        .IF g_hasInitTimer == 0
            invoke GetClientRect, hWnd, offset g_winRect
            invoke SetTimer, hWnd, 1, 1000, NULL
            mov g_hasInitTimer, 1
        .ENDIF
    .ELSEIF message == WM_TIMER
        .IF wParam == 1
            invoke InvalidateRect, hWnd, NULL, FALSE
        .ENDIF
    .ELSE
        invoke DefWindowProc, hWnd, message, wParam, lParam
        ret
    .ENDIF

    xor eax, eax
    ret
WndProc endp

WinMain proc hInstance: HINSTANCE
    local @wc:WNDCLASS
    local @hWnd:HWND
    local @msg:MSG
    
    ; 初始化
    invoke RtlZeroMemory, addr @wc, size @wc
    invoke RtlZeroMemory, addr @msg, size @msg
    mov @hWnd, 0
        
    ; 注册窗口类
    mov @wc.style,  CS_HREDRAW and CS_VREDRAW
    mov @wc.lpfnWndProc, offset WndProc
    mov eax, hInstance
    mov @wc.hInstance, eax
    invoke LoadIcon, NULL, IDI_APPLICATION
    mov @wc.hIcon, eax
    invoke LoadCursor, NULL, IDC_ARROW
    mov @wc.hCursor, eax
    mov @wc.hbrBackground, COLOR_WINDOW + 1
    mov @wc.lpszClassName, offset g_szWindowClassName

    invoke RegisterClass, addr @wc

    ; 创建窗口实例
    ; 右斜杠表示换行
    invoke CreateWindowEx, 0, offset g_szWindowClassName, offset g_szWindowTitle, \
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, \
        NULL, NULL, hInstance, NULL
    mov @hWnd, eax
    invoke ShowWindow, @hWnd, SW_SHOW
    invoke UpdateWindow, @hWnd

    ; 消息循环
    .WHILE TRUE
        invoke GetMessage, addr @msg, NULL, 0, 0
        .BREAK .IF eax == 0
        
        invoke TranslateMessage, addr @msg
        invoke DispatchMessage, addr @msg
    .ENDW

    ret
WinMain endp

ENTRY:
    invoke GetModuleHandle, NULL
    invoke WinMain, eax

    invoke ExitProcess, eax
end ENTRY