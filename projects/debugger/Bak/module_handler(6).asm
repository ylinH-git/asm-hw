.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc
include pe_handler.inc

.data
	g_szModuleListHeader	db "��ַ        ģ������",0dh, 0ah, 0
	g_szModuleListFormat	db "0x%08X  %s", 0dh, 0ah, 0
.code 

ShowModuleList proc uses ecx esi
	LOCAL @importDirectoryLen: DWORD
	LOCAL @importDllNameFAsAddr: DWORD
	LOCAL @pFile:DWORD
	LOCAL @dllName[256]:BYTE
	LOCAL @currDllAddr: HMODULE
	LOCAL @tempName[256]:BYTE
	
	invoke crt_printf, offset g_szModuleListHeader
	invoke GetImportDirectoryLen
	mov @importDirectoryLen, eax
	invoke GetImportDllNameFAsAddr
	mov @importDllNameFAsAddr, eax
	invoke GetFileAddr
	mov @pFile, eax
	
	
	xor ecx, ecx
	mov esi, @importDllNameFAsAddr
	.while ecx < @importDirectoryLen
		push ecx
		invoke RtlZeroMemory, addr @dllName, 256
		invoke RtlZeroMemory, addr @tempName, 256
		invoke crt_fseek, g_pFile, esi, SEEK_SET
		invoke crt_fread, addr @tempName, 1, 256, g_pFile
		invoke crt_strcpy, addr @dllName, addr @tempName
		invoke GetModuleHandle,addr @dllName, addr @tempName
		mov @currDllAddr, eax
		
		invoke crt_printf, offset g_szModuleListFormat, @currDllAddr, addr @dllName
		pop ecx
		
		
		add esi, size DWORD 
		inc ecx
	.endw
	ret

ShowModuleList endp
end