.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include asmtool.inc

includelib XEDParse_x86.lib

XEDPARSE struc 8
    x64         db 0; // use 64-bit instructions
    cip         dq 0; //instruction pointer (for relative addressing)
    dest_size   dd 0; //destination size (returned by XEDParse)
    cbUnknown   dd 0; //unknown operand callback
    dest        db 16  dup(0); //destination buffer
    instr_      db 256 dup(0); //instruction text
    error       db 256 dup(0); //error text (in case of an error)
XEDPARSE ends


XEDParseAssemble proto C XEDParse:ptr XEDPARSE

.data
	g_xde XEDPARSE  <?>
	g_dwPid dd 0
	g_szAsm db 256 dup(0)
	g_dwEAsmHandle HWND 0
	g_dwLineCount dd 0
	g_dwCurLine dd 0
	g_dwCurSize dd 0
	g_szDest db 16 dup(0)
	g_szError db "注入失败", 0
.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;########################################################################



Inject proc hWin:HWND
	LOCAL @hProc:HANDLE
	LOCAL @pAddr:LPVOID
	LOCAL @curAddr:LPVOID
	LOCAL @dwBytesWrited:DWORD
	
	mov g_dwLineCount, 0
	mov g_dwCurLine, 0
	invoke GetDlgItemInt, hWin, EDT_PID, NULL, FALSE
	mov g_dwPid, eax
  	invoke GetDlgItem,hWin, EDT_ASM
  	mov g_dwEAsmHandle, eax
	invoke SendMessage, g_dwEAsmHandle, EM_GETLINECOUNT, NULL, NULL
	mov g_dwLineCount, eax
  
  	
  		; 打开进程
	invoke OpenProcess,PROCESS_VM_OPERATION  or PROCESS_VM_WRITE or PROCESS_VM_READ, FALSE, g_dwPid
	mov @hProc, eax
	
	; 申请远程进程中的内存空间
	invoke VirtualAllocEx,@hProc, NULL, 1, MEM_RESERVE or MEM_COMMIT, PAGE_EXECUTE_READWRITE
	mov @pAddr, eax
	mov @curAddr, eax
  	.while g_dwLineCount != 0
  		mov g_dwCurSize, 0
  		mov @dwBytesWrited, 0
  		invoke RtlZeroMemory,offset g_szDest, 16
  		invoke RtlZeroMemory,offset g_szAsm, 256
  		
  		invoke SendMessage, g_dwEAsmHandle, EM_LINELENGTH, g_dwCurLine, NULL
  		
  		mov g_szAsm[0], al
  		invoke SendMessage, g_dwEAsmHandle, EM_GETLINE, g_dwCurLine, offset g_szAsm
  		
  		mov g_xde.x64, 0
    	lea eax, g_xde.instr_
    	invoke crt_strcpy, eax, offset g_szAsm
    	invoke XEDParseAssemble, offset g_xde
    	.if eax == 0
    		invoke MessageBox,hWin, offset g_szError, NULL, MB_OK
    		ret
    	.endif
    	lea eax, g_xde.dest
    	invoke crt_strcpy, offset g_szDest, eax
    	mov eax,  g_xde.dest_size
  		mov g_dwCurSize, eax
	
		; 代码写入目标进程
		invoke WriteProcessMemory, @hProc, @curAddr, offset g_szDest, g_dwCurSize, addr @dwBytesWrited
  		
		
		mov eax, @dwBytesWrited
  		add @curAddr, eax
  		
  		dec g_dwLineCount
  		inc g_dwCurLine
  	.endw
  	
  	
  	; 通过线程启动注入的代码
    invoke CreateRemoteThread, @hProc, NULL, 0, @pAddr, NULL, NULL, NULL

	ret

Inject endp


DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG

	.elseif eax==WM_COMMAND
		mov eax, wParam
		.if ax == BTN_INJECT
			invoke Inject, hWin
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
