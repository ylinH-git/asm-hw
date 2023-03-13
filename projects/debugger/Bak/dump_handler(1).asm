.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szDump db "_dump", 0
	g_szDumpFileName db 256 dup(0)
	g_readFileBuf db 10000h dup(0)
.code 


DumpPe proc pFilePath:DWORD
	LOCAL @hFile:HANDLE
	LOCAL @hDumpFile:HANDLE
    LOCAL @hMap:HANDLE
    LOCAL @pAddr:LPVOID
    LOCAL @pDosHdr:ptr IMAGE_DOS_HEADER
    LOCAL @pNtHdr:ptr IMAGE_NT_HEADERS
    LOCAL @pSecHdr:ptr IMAGE_SECTION_HEADER
    LOCAL @dwNumOfSecs:DWORD
    LOCAL @dwOldProc:DWORD
    LOCAL @pImp:ptr IMAGE_IMPORT_DESCRIPTOR
    LOCAL @impEmpty: IMAGE_IMPORT_DESCRIPTOR
    LOCAL @hDll:HMODULE
    LOCAL @pINT:LPVOID
    LOCAL @pIAT:LPVOID
    LOCAL @copyLen:DWORD
    LOCAL @dwBytesReaded:DWORD
	LOCAL @dwBytesWrited:DWORD
	
	invoke RtlZeroMemory, g_szDumpFileName, 256
	invoke RtlZeroMemory,addr @impEmpty, size IMAGE_IMPORT_DESCRIPTOR
    
    invoke GetFileName
    invoke crt_strcpy, offset g_szDumpFileName, eax
    invoke crt_strcat, offset g_szDumpFileName, offset g_szDump
    invoke GetFileExt
    invoke crt_strcat, offset g_szDumpFileName, eax
    
    
    ;映射读入文件
    invoke CreateFile, pFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL

    .if eax == INVALID_HANDLE_VALUE
    	invoke crt_printf, offset g_szFileErrorTip
    	ret
    .endif
    mov @hFile, eax
    ;映射dump文件
    invoke CreateFile, offset g_szDumpFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    .if eax == INVALID_HANDLE_VALUE
    	invoke crt_printf, offset g_szFileCreateErrorTip
    	ret
    .endif
    mov @hDumpFile, eax
    
    invoke CreateFileMapping, @hFile, NULL, PAGE_READONLY, 0, 0, NULL
    mov @hMap, eax
    
    invoke MapViewOfFile, @hMap, FILE_MAP_READ, 0, 0, 0
    mov @pAddr, eax
    
    ;解析格式
    mov eax, @pAddr
    mov @pDosHdr, eax
    
    mov esi, @pDosHdr
    assume esi:ptr IMAGE_DOS_HEADER
    mov eax, [esi].e_lfanew
    add eax, @pAddr
    mov @pNtHdr, eax
    
    mov eax, [esi].e_lfanew
    mov @copyLen, eax
    invoke RtlZeroMemory, offset g_readFileBuf, 10000h
    invoke ReadFile, @hFile, offset g_readFileBuf, @copyLen, addr @dwBytesReaded, NULL
    invoke WriteFile, @hFile, offset g_readFileBuf, @copyLen, addr @dwBytesWrited, NULL
    
    mov esi, @pNtHdr
    assume esi:ptr IMAGE_NT_HEADERS
    lea eax, [esi].OptionalHeader
    movzx ecx, [esi].FileHeader.SizeOfOptionalHeader
    add eax, ecx
    mov @pSecHdr, eax
    
    movzx eax, [esi].FileHeader.NumberOfSections
    mov @dwNumOfSecs, eax
    
    mov eax, [esi].OptionalHeader.AddressOfEntryPoint
    add eax, hInst
	
	
	ret

DumpPe endp
	

end