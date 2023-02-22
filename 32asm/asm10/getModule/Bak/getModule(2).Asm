.586
.model flat,stdcall
option casemap:none

   include windows.inc
   include user32.inc
   include kernel32.inc
   include msvcrt.inc
   
   includelib user32.lib
   includelib kernel32.lib
   includelib msvcrt.lib

assume fs:nothing

LDR_DATA struc
	Flink DWORD 0
	Blink DWORD 0
	UnUsed DWORD 4 dup(0)
	ImageBase DWORD 0
	EntryPoint DWORD 0
	ImageSize DWORD 0
	UnUsed1 DWORD 0
	ImagePath DWORD 0
	UnUsed2 DWORD 0
	ImageName DWORD 0
LDR_DATA ends

.data
	g_szModuleName db "KERNEL32", 0
	g_szModuleNameW db 255 dup(0)
	g_szModuleNameUser32 db "user32", 0
.code

MyGetModuleHandle proc uses ecx ebx lpModuleName: DWORD
	LOCAL @pModuleName: DWORD
	LOCAL @dwNameLen: DWORD
	.if lpModuleName == NULL
		; 取出peb
		mov eax, dword ptr fs:[30h]
		; 取出ImageBaseAddress
		mov eax, dword ptr [eax +8]
		ret
	.endif
	
	invoke crt_strlen, offset g_szModuleName
	mov @dwNameLen, eax
	invoke MultiByteToWideChar, 65001, MB_COMPOSITE, offset g_szModuleName, @dwNameLen, offset g_szModuleNameW, 255
	
	mov ecx, dword ptr fs:[30h]
	; 取出LDR
	mov ecx, dword ptr [ecx + 0ch]
	; 取出InLoadOrderModuleList第一个flink（即主模块相关信息），并备份
	mov ecx, dword ptr [ecx + 0ch]
	mov ebx, ecx
	assume ecx:ptr LDR_DATA
	.while [ecx].Flink != ebx
		mov eax, dword ptr [ecx].ImageName
		mov @pModuleName, eax
		push ecx
		invoke crt_wcsstr, @pModuleName, offset g_szModuleNameW 
		pop ecx
		.if eax != NULL
			mov eax, [ecx].ImageBase
			ret
		.endif
		mov ecx, [ecx].Flink
	.endw
	mov eax, 0
	ret
MyGetModuleHandle endp

MyGetModuleFileName proc hModule:HMODULE, lpFilename: DWORD, nSize:DWORD
	mov ecx, dword ptr fs:[30h]
	; 取出LDR
	mov ecx, dword ptr [ecx + 0ch]
	; 取出InLoadOrderModuleList第一个flink（即主模块相关信息），并备份
	mov ecx, dword ptr [ecx + 0ch]
	mov ebx, ecx
	assume ecx:ptr LDR_DATA
	.while [ecx].Flink != ebx
		mov eax, dword ptr [ecx].ImageBase
		.if hModule == eax
			invoke crt_wcscmp,  offset g_szModuleNameW, [ecx].ImageName
			ret
		.endif
		mov ecx, [ecx].Flink
	.endw
	mov eax, 0
	ret
MyGetModuleFileName endp

start:
    invoke GetModuleHandle, NULL
    invoke MyGetModuleHandle, NULL
    invoke GetModuleHandle, offset g_szModuleName
    invoke MyGetModuleHandle, offset g_szModuleName
    
    invoke GetModuleHandle, offset g_szModuleNameUser32
    mov ebx, eax
    invoke MyGetModuleFileName, ebx, offset g_szModuleNameW, 255
 
	invoke ExitProcess,eax
	
end start
