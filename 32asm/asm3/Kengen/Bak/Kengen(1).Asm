.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include Kengen.inc

.data
	g_szInput db 256 dup(0)
	g_szOutput db 256 dup(0)
	g_szTable db "0123456789", 0
	g_szSplit db '-',0
	g_szHead db "6769-", 0
	g_dwCurrIndex dd 0
	g_dwTableLen dd 0
	g_curStr db 2 dup(0)
.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;########################################################################
KenGen proc hWin:HWND
	LOCAL @oldEbp:DWORD
	mov @oldEbp, ebp
	mov g_dwCurrIndex, 0
	mov g_dwTableLen, 9
	invoke 	RtlZeroMemory, offset g_curStr, 2
	invoke 	RtlZeroMemory, offset g_szOutput, 256
	invoke lstrcpy,offset g_szOutput, offset g_szHead
	
	; »ñÈ¡ÊäÈë×Ö·û´®
	invoke 	GetDlgItemText,hWin, EDT_INPUT, offset g_szInput, size g_szInput
	mov     edi, offset g_szInput
	or 		ecx, 0FFFFFFFFh
	xor     eax, eax
	repne   scas byte ptr es:[edi]
	not     ecx
	dec     ecx
  	cmp 	ecx, 8
  	jge 	L014
  	invoke 	RtlZeroMemory, offset g_szOutput, 256
  	invoke 	SetDlgItemText,hWin, EDT_OUTPUT, offset g_szOutput
  	ret
L014:
  	mov     ecx, offset g_szOutput
  	xor     esi, esi
  	mov     g_dwCurrIndex, esi
L025:
  	mov     ebp, offset g_szTable
  	or      ecx, 0FFFFFFFFh
  	mov     edi, ebp
  	xor     eax, eax
  	repne   scas byte ptr es:[edi]
  	not     ecx
  	dec     ecx
  	mov     eax, esi
  	xor     edx, edx
  	mov     edi, ebp
  	div     ecx
  	mov     ecx, g_dwTableLen
  	mov     eax, esi
  	movsx   ebx, byte ptr [edx+ebp]
  	xor     edx, edx
  	div     ecx
  	movsx   eax, byte ptr [edx+offset g_szInput]
  	lea     edx, dword ptr [eax+eax*2]
  	lea     eax, dword ptr [eax+edx*4]
  	mov     edx, esi
  	imul    edx, esi
  	imul    edx, esi
  	imul    edx, ecx
  	add     ebx, eax
  	or      ecx, 0FFFFFFFFh
  	xor     eax, eax
  	add     ebx, edx
  	repne   scas byte ptr es:[edi]
  	not     ecx
  	dec     ecx
  	mov     eax, ebx
  	xor     edx, edx
  	div     ecx
  	lea     ecx, offset g_szOutput
  	mov     al, byte ptr [edx+ebp]
  	mov 	g_curStr[0], al
  	invoke 	lstrcat, offset g_szOutput, offset g_curStr
  	test    esi, esi
  	je 		L073
  	mov     eax, esi
  	xor     edx, edx
  	mov     ecx, 3
  	div     ecx
  	test    edx, edx
  	jnz 	L073
  	mov     ecx, offset g_dwTableLen
  	invoke  lstrcat, offset g_szOutput, offset g_szSplit
L073:
  	inc     esi
  	cmp     esi, 9
  	jb L025
  	
  	mov ebp, @oldEbp
  	invoke 	SetDlgItemText,hWin, EDT_OUTPUT, offset g_szOutput

	ret

KenGen endp


DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG

	.elseif eax==WM_COMMAND
		mov eax, wParam
		.if ax == BTN_GEN
			invoke KenGen, hWin
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
