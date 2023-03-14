.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szBlock db " ", 0
.code 

HandlerECommand proc uses ecx ebx edx hProc:HANDLE, dwAddr:DWORD, pStr:DWORD
	LOCAL @currVal:DWORD
	invoke crt_strtok, pStr, offset g_szBlock
	mov edx, eax
	mov ecx, dwAddr
	.while edx != NULL
		push ecx
		invoke crt_strtoul, edx, NULL, 16
		pop ecx
        .if (eax == 0 && byte ptr [edx] != '0') || eax == -1
        	ret
        .elseif
        	mov @currVal, eax
          	invoke WriteMemory, hProc, ecx, addr @currVal, 1
        .endif
		
		push ecx
		invoke crt_strtok, NULL, offset g_szBlock
		pop ecx
		mov edx, eax
		inc ecx
	.endw
    ret

HandlerECommand endp


end