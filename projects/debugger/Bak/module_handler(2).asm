.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc
include pe_handler.inc

.data
	g_szModuleListHeader	db "µØÖ·        Ä£¿éÃû³Æ",0dh, 0ah, 0
	g_szModuleListFormat	db "0x%08X  %s", 0dh, 0ah, 0
	g_szModuleName          db 256 dup(0)
.code 

ShowModuleList proc uses ecx esi ebx hProc:DWORD
	LOCAL @importDirectoryLen: DWORD
	LOCAL @importDllNameFAsAddr: DWORD
	LOCAL @pFile:DWORD
	LOCAL @dllName[256]:BYTE
	LOCAL @currDllAddr: HMODULE
	LOCAL @tempName[256]:BYTE
	LOCAO @hModuleNeedByte:DWORD
	
	invoke RtlZeroMemory, offset g_szModuleName, 256
	
	invoke crt_printf, offset g_szModuleListHeader
	invoke GetImportDirectoryLen
	mov @importDirectoryLen, eax
	invoke GetImportDllNameFAsAddr
	mov @importDllNameFAsAddr, eax
	invoke GetFileAddr
	mov @pFile, eax
	
	invoke GetFileName
	invoke crt_strcpy, offset g_szModuleName, eax
	invoke GetFileExt
	invoke crt_strcat, offset g_szModuleName, eax
	
	invoke EnumProcessModules, hProc, addr @currDllAddr, 4, addr @hModuleNeedByte
	invoke GetModuleHandle, NULL
	mov @currDllAddr, eax
	invoke crt_printf, offset g_szModuleListFormat, @currDllAddr, offset g_szModuleName
	
	xor ecx, ecx
	mov esi, @importDllNameFAsAddr
	.while ecx < @importDirectoryLen
		push ecx
		invoke RtlZeroMemory, addr @dllName, 256
		invoke RtlZeroMemory, addr @tempName, 256
		mov ebx, dword ptr [esi]
		invoke crt_fseek, @pFile, ebx, SEEK_SET
		invoke crt_fread, addr @tempName, 1, 256, @pFile
		invoke crt_strcpy, addr @dllName, addr @tempName
		invoke GetModuleHandle,addr @dllName
		mov @currDllAddr, eax
		
		invoke crt_printf, offset g_szModuleListFormat, @currDllAddr, addr @dllName
		pop ecx
		
		
		add esi, size DWORD 
		inc ecx
	.endw
	ret

ShowModuleList endp
end