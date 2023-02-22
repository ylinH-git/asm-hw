.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include inject.inc

; 计算数据在被注入程序中的位置
getDataRealAddr MACRO pData
	mov eax, offset pData
	add eax, ebx
endm

pushData MACRO pData
	getDataRealAddr pData
	push eax
endm


callFunc macro  pFunc
    mov eax, offset pFunc
    add eax, ebx
    call dword ptr [eax]
endm

.data
	g_szWinmine		db "扫雷", 0
	g_szUser32 		db "user32", 0
	g_szNtdll 		db "ntdll", 0
	g_szKernal32    db "kernel32", 0
	g_szMessageBox  db "MessageBoxA", 0
	g_szLoadIcon  db "LoadIconA", 0
	g_szLoadCursor  db "LoadCursorA", 0
	g_szRegisterClass  db "RegisterClassA", 0
	g_szCreateWindowEx db "CreateWindowExA", 0
	g_szShowWindow  db "ShowWindow", 0
	g_szUpdateWindow  db "UpdateWindow", 0
	g_szGetMessage  db "GetMessageA", 0
	g_szRtlZeroMemory  db "RtlZeroMemory", 0
	g_szTranslateMessage  db "TranslateMessage", 0
	g_szDispatchMessage  db "DispatchMessageA", 0
	g_szPostQuitMessage  db "PostQuitMessage", 0
	g_szDefWindowProc  db "DefWindowProcA", 0
	g_szGetModuleHandle db "GetModuleHandleA", 0
	g_szFindWindow db "FindWindowA", 0
.code


CODE_BEGIN:
	int 3 ; 抛出异常，可以用于调试时中断程序
	jmp INJECT_CODE
	g_szText	db "你被注入了，扫雷", 0
	g_szTip     db "温馨提示", 0
	g_szWinmineStr		db "扫雷", 0
	g_szWindowClassName db "injectwindow", 0 
	g_pfnMessageBox dd 0
	g_pfnLoadIcon dd 0
	g_pfnLoadCursor dd 0
	g_pfnRegisterClass dd 0
	g_pfnShowWindow dd 0
	g_pfnUpdateWindow  dd 0
	g_pfnGetMessage dd 0
	g_pfnRtlZeroMemory dd 0
	g_pfnTranslateMessage dd 0
	g_pfnDispatchMessage dd 0
	g_pfnCreateWindowEx dd 0
    g_pfnPostQuitMessage dd 0
	g_pfnDefWindowProc dd 0
	g_pfnGetModuleHandle dd 0
	g_pfnFindWindow dd 0
	
	
WndProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:WPARAM
	; 核心代码，快速获取当前代码位置，用于重定位
	call NEXT1
NEXT1:
	pop ebx
	sub ebx, offset NEXT1 ;计算偏移
	.IF uMsg == WM_DESTROY
		push 0
		callFunc g_pfnPostQuitMessage
    .ELSE
    	push lParam
    	push wParam
    	push uMsg
    	push hWnd
    	callFunc g_pfnDefWindowProc
        ret
    .ENDIF
    xor eax, eax
	ret
WndProc endp


RunWindow proc
    LOCAL @wc:WNDCLASS
    LOCAL @hWnd:HWND
    LOCAL @msg:MSG
    LOCAL @hInstance:HINSTANCE
    LOCAL @pHwnd:HWND
    
    pushData g_szWinmineStr
    push NULL
    callFunc g_pfnFindWindow
    mov @pHwnd, eax
    
    push NULL
    callFunc g_pfnGetModuleHandle
    mov @hInstance, eax
    ; 初始化
    mov eax, size @wc
    push eax
    lea eax, @wc
    push eax
    callFunc g_pfnRtlZeroMemory
    
    mov eax, size @msg
    push eax
    lea eax, @msg
    push eax
    callFunc g_pfnRtlZeroMemory
    
    mov @hWnd, 0
        
    ; 注册窗口类
    mov @wc.style,  CS_HREDRAW and CS_VREDRAW
    getDataRealAddr WndProc
    mov @wc.lpfnWndProc, eax
    mov eax, @hInstance
    mov @wc.hInstance, eax
    push IDI_APPLICATION
    push NULL
    callFunc g_pfnLoadIcon
    mov @wc.hIcon, eax
    push IDC_ARROW
    push NULL
    callFunc g_pfnLoadCursor
    mov @wc.hCursor, eax
    mov @wc.hbrBackground, COLOR_WINDOW + 1
    getDataRealAddr g_szWindowClassName
    mov @wc.lpszClassName, eax
	
	lea eax, @wc
	push eax
	callFunc g_pfnRegisterClass
	
	push NULL
	push @hInstance
	push NULL
	push @pHwnd
	push 0
	push CW_USEDEFAULT
	push 0
	push CW_USEDEFAULT
	push WS_OVERLAPPEDWINDOW or WS_VISIBLE or WS_POPUP
	pushData g_szTip
	pushData g_szWindowClassName
	push 0
	callFunc g_pfnCreateWindowEx
    
    mov @hWnd, eax
    
    push SW_SHOW
    push @hWnd
    callFunc g_pfnShowWindow
    
    push @hWnd
    callFunc g_pfnUpdateWindow

    ; 消息循环
    .WHILE TRUE
    	push 0
    	push 0
    	lea eax, @msg
    	push eax
    	 callFunc g_pfnGetMessage
        .BREAK .IF eax == 0
        
        lea eax, @msg
    	push eax
    	callFunc g_pfnTranslateMessage
    	lea eax, @msg
    	push eax
    	callFunc g_pfnDispatchMessage
    .ENDW

    ret
RunWindow endp

INJECT_CODE:
	; 核心代码，快速获取当前代码位置，用于重定位
	call NEXT
NEXT:
	pop ebx
	sub ebx, offset NEXT ;计算偏移
	
	; 调用MessageBox
	; invoke call jmp这类的指令都是跳转到指定的偏移
    ; 而offset这类是计算出的常量，所以传地址时会原样复制到目标进程
	invoke RunWindow
    
	ret
	

CODE_END:


Inject proc
	LOCAL @hWnd:HWND
	LOCAL @dwPid:DWORD
	LOCAL @hProc:HANDLE
	LOCAL @pAddr:LPVOID
	LOCAL @dwBytesWrited:DWORD
	LOCAL @hUser32:HANDLE
	LOCAL @hNtdll:HANDLE
	LOCAL @hKernal32:HANDLE
    LOCAL @dwOldProc:DWORD
    LOCAL @pfnMessageBox:DWORD
    LOCAL @pfnLoadIcon:DWORD
    LOCAL @pfnLoadCursor:DWORD
    LOCAL @pfnRegisterClass:DWORD
    LOCAL @pfnCreateWindowEx:DWORD
    LOCAL @pfnShowWindow:DWORD
    LOCAL @pfnUpdateWindow:DWORD
    LOCAL @pfnGetMessage:DWORD
    LOCAL @pfnRtlZeroMemory:DWORD
    LOCAL @pfnTranslateMessage:DWORD
    LOCAL @pfnDispatchMessage:DWORD
    LOCAL @pfnPostQuitMessage:DWORD
    LOCAL @pfnDefWindowProc:DWORD
    LOCAL @pfnGetModuleHandle:DWORD
    LOCAL @pfnFindWindow:DWORD

	
	; 准备工作, 用于获取dll库函数地址
	; 获取模块句柄
    invoke GetModuleHandle, offset g_szUser32
    mov @hUser32, eax
    
   	invoke GetModuleHandle, offset g_szNtdll
    mov @hNtdll, eax
    
    invoke GetModuleHandle, offset g_szKernal32
    mov @hKernal32, eax
    
    ; 获取模块中相应函数的地址
    invoke GetProcAddress, @hKernal32, offset g_szGetModuleHandle
    mov @pfnGetModuleHandle, eax
    
    invoke GetProcAddress, @hUser32, offset g_szFindWindow
    mov @pfnFindWindow, eax
    
    invoke GetProcAddress, @hUser32, offset g_szMessageBox
    mov @pfnMessageBox, eax
    
    invoke GetProcAddress, @hUser32, offset g_szLoadIcon
    mov @pfnLoadIcon, eax
    
   	invoke GetProcAddress, @hUser32, offset g_szLoadCursor
    mov @pfnLoadCursor, eax
    
    invoke GetProcAddress, @hUser32, offset g_szRegisterClass
    mov @pfnRegisterClass, eax
    
    invoke GetProcAddress, @hUser32, offset g_szCreateWindowEx
    mov @pfnCreateWindowEx, eax
    
    invoke GetProcAddress, @hUser32, offset g_szShowWindow
    mov @pfnShowWindow, eax
    
   	invoke GetProcAddress, @hUser32, offset g_szUpdateWindow
    mov @pfnUpdateWindow, eax
    
    invoke GetProcAddress, @hUser32, offset g_szTranslateMessage
    mov @pfnTranslateMessage, eax
    
    invoke GetProcAddress, @hUser32, offset g_szGetMessage
    mov @pfnGetMessage, eax
    
   	invoke GetProcAddress, @hNtdll, offset g_szRtlZeroMemory
    mov @pfnRtlZeroMemory, eax
    
    invoke GetProcAddress, @hUser32, offset g_szDispatchMessage
    mov @pfnDispatchMessage, eax
    
   	invoke GetProcAddress, @hUser32, offset g_szPostQuitMessage
    mov @pfnPostQuitMessage, eax
    
    invoke GetProcAddress, @hUser32, offset g_szDefWindowProc
    mov @pfnDefWindowProc, eax   
     
     
    invoke VirtualProtect, offset g_pfnGetModuleHandle, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnGetModuleHandle
    mov g_pfnGetModuleHandle, eax
    invoke VirtualProtect, offset g_pfnGetModuleHandle, 1, @dwOldProc, addr @dwOldProc
    
    
   	invoke VirtualProtect, offset g_pfnFindWindow, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnFindWindow
    mov g_pfnFindWindow, eax
    invoke VirtualProtect, offset g_pfnFindWindow, 1, @dwOldProc, addr @dwOldProc
    
    ; 修改内存块权限，这是因为我们的函数代码卸载.code模块下面，默认是不可以写的
    invoke VirtualProtect, offset g_pfnMessageBox, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnMessageBox
    mov g_pfnMessageBox, eax
    ; 还原内存块权限
    invoke VirtualProtect, offset g_pfnMessageBox, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnLoadIcon, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnLoadIcon
    mov g_pfnLoadIcon, eax
    invoke VirtualProtect, offset g_pfnLoadIcon, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnLoadCursor, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnLoadCursor
    mov g_pfnLoadCursor, eax
    invoke VirtualProtect, offset g_pfnLoadCursor, 1, @dwOldProc, addr @dwOldProc

    invoke VirtualProtect, offset g_pfnRegisterClass, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnRegisterClass
    mov g_pfnRegisterClass, eax
    invoke VirtualProtect, offset g_pfnRegisterClass, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnCreateWindowEx, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnCreateWindowEx
    mov g_pfnCreateWindowEx, eax
    invoke VirtualProtect, offset g_pfnCreateWindowEx, 1, @dwOldProc, addr @dwOldProc
        
    invoke VirtualProtect, offset g_pfnShowWindow, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnShowWindow
    mov g_pfnShowWindow, eax
    invoke VirtualProtect, offset g_pfnShowWindow, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnUpdateWindow, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnUpdateWindow
    mov g_pfnUpdateWindow, eax
    invoke VirtualProtect, offset g_pfnUpdateWindow, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnTranslateMessage, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnTranslateMessage
    mov g_pfnTranslateMessage, eax
    invoke VirtualProtect, offset g_pfnTranslateMessage, 1, @dwOldProc, addr @dwOldProc
        
    invoke VirtualProtect, offset g_pfnGetMessage, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnGetMessage
    mov g_pfnGetMessage, eax
    invoke VirtualProtect, offset g_pfnGetMessage, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnRtlZeroMemory, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnRtlZeroMemory
    mov g_pfnRtlZeroMemory, eax
    invoke VirtualProtect, offset g_pfnRtlZeroMemory, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnDispatchMessage, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnDispatchMessage
    mov g_pfnDispatchMessage, eax
    invoke VirtualProtect, offset g_pfnDispatchMessage, 1, @dwOldProc, addr @dwOldProc

	invoke VirtualProtect, offset g_pfnPostQuitMessage, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnPostQuitMessage
    mov g_pfnPostQuitMessage, eax
    invoke VirtualProtect, offset g_pfnPostQuitMessage, 1, @dwOldProc, addr @dwOldProc
    
    invoke VirtualProtect, offset g_pfnDefWindowProc, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
    mov eax, @pfnDefWindowProc
    mov g_pfnDefWindowProc, eax
    invoke VirtualProtect, offset g_pfnDefWindowProc, 1, @dwOldProc, addr @dwOldProc
    

	; 远程线程注入
	; 获取窗口句柄
	invoke FindWindow, NULL, offset g_szWinmine
	mov @hWnd, eax
	
	; 获取进程ID
	invoke GetWindowThreadProcessId, @hWnd, addr @dwPid
	
	; 打开进程
	invoke OpenProcess,PROCESS_ALL_ACCESS, FALSE, @dwPid
	mov @hProc, eax
	
	; 申请远程进程中的内存空间
	invoke VirtualAllocEx,@hProc, NULL, 1, MEM_RESERVE or MEM_COMMIT, PAGE_EXECUTE_READWRITE
	mov @pAddr, eax
	
	; 代码写入目标进程
	invoke WriteProcessMemory,@hProc, @pAddr, offset CODE_BEGIN, offset CODE_END - offset CODE_BEGIN, addr @dwBytesWrited
	
	; 通过线程启动注入的代码
    invoke CreateRemoteThread, @hProc, NULL, 0, @pAddr, NULL, NULL, NULL
	
	ret

Inject endp

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;########################################################################


DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG

	.elseif eax==WM_COMMAND
		mov eax, wParam
		.if ax == BTN_INJECT
			invoke Inject
		.endif

	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

DlgProc endp

end start
