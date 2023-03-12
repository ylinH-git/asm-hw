.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szShowBufAddrFormat db "0x%08X", 0
	g_szOneByteFormat db " %02X", 0
	g_szSplit db " -", 0
	g_szByteFomat db "%c", 0
	g_szPoint db ".", 0
	g_szBlock db "   ", 0
.code 

PrintFormatBytesToAscii proc uses ecx ebx pBytes:DWORD
	mov ebx, pBytes
	xor ecx, ecx
	.while ecx < 16
		.if byte ptr [ebx] == 0
			push ecx
			invoke crt_printf, offset g_szByteFormat, offset g_szPoint
			pop ecx
		.elseif
			push ecx
			xor ecx, ecx
			mov cl, byte ptr [ebx]
			invoke crt_printf, offset g_szByteFormat, ecx
			pop ecx
		.endif
		inc ebx
		inc ecx
	.endw
	ret

PrintFormatBytesToAscii endp

HandlerDDCommand proc uses ecx ebx edx hProc:HANDLE, dwAddr:DWORD
	LOCAL @currAddr:DWORD
	LOCAL @dwByteWirtten: DWORD
	LOCAL @currBytes[16]:BYTE
	
	invoke RtlZeroMemory, addr @currBytes, 16
	
	mov eax, dwAddr
	mov @currAddr, eax
	
	xor ecx, ecx
	.while ecx < 8
		push ecx
		invoke ReadProcessMemory,hProc, @currAddr, addr @currBytes, 16,addr @dwByteWirtten
		pop ecx
		
		push ecx
		invoke crt_printf, offset g_szShowBufAddrFormat, @currAddr
		pop ecx
		xor edx, edx
		lea ebx, @currBytes
		.while edx < 16
			.if edx == 8
				push ecx
				push edx
				invoke crt_printf, offset g_szSplit
				pop edx
				pop ecx
			.endif
			push ecx
			push edx
			xor ecx, ecx
			mov cl, byte ptr [ebx]
			invoke crt_printf, offset g_szOneByteFormat, ecx
			pop edx
			pop ecx
			
			inc edx
			inc ebx
		.endw
		push ecx
		invoke crt_printf, offset g_szBlock
		pop ecx
		invoke PrintFormatBytesToAscii, addr @currBytes
		push ecx
		invoke crt_printf, offset g_szRn
		pop ecx 
		inc ecx
		add @currAddr, 16
	.endw
	
	 
    ret

HandlerDDCommand endp


end