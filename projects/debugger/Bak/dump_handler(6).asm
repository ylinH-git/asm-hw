.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

sPeFileSection struc
	sectionHeader IMAGE_SECTION_HEADER <<0>>
	data dd 0
	dataSize dd 0
	normalSize dd 0
sPeFileSection ends

.data
	g_szDump db "_dump", 0
	g_szDumpDone db "Dump 完成", 0
	g_szDumpFileName db 256 dup(0)
	g_hModuleBase dd 0
	g_fileMemory dd 0
	g_fileHeaderMemory dd 0  
	g_pDosHeader dd 0
	g_pDosStub dd 0
	g_dosStubSize dd 0
	g_pNTHeader dd 0
	g_overlayData dd 0
	g_overlaySize dd 0	
	g_pHeaderMemory dd 0
	g_pListPeSection dd 0

.code
GetNumOfSections proc uses esi
	mov esi, g_pNTHeader
	assume esi: ptr IMAGE_NT_HEADERS
	
	xor eax, eax
	mov ax, [esi].FileHeader.NumberOfSections

	ret
GetNumOfSections endp

GetDosAndNtHeader proc uses esi dwSize:DWORD
	mov eax, g_pHeaderMemory
	mov g_pDosHeader, eax
	mov esi, g_pDosHeader
	assume esi:ptr IMAGE_DOS_HEADER
	
	mov eax, dwSize
	.if [esi].e_lfanew > 0 && [esi].e_lfanew < eax
		mov eax, g_pDosHeader
		add eax, [esi].e_lfanew
		mov g_pNTHeader, eax
		
		.if [esi].e_lfanew > size IMAGE_DOS_HEADER
			mov eax, [esi].e_lfanew
			sub eax, size IMAGE_DOS_HEADER
			mov g_dosStubSize, eax
			
			mov eax, g_pDosHeader
			add eax, size IMAGE_DOS_HEADER
			mov g_pDosStub, eax
		.elseif [esi].e_lfanew < size IMAGE_DOS_HEADER
			mov [esi].e_lfanew, size IMAGE_DOS_HEADER
		.endif
	.endif
	
	ret

GetDosAndNtHeader endp

CalcCorrectPeHeaderSize proc uses esi ecx ebx
	LOCAL @correctSize:DWORD
	mov esi, g_pDosHeader
	assume esi:ptr IMAGE_DOS_HEADER
	
	mov ecx, [esi].e_lfanew
	add ecx, 50
	invoke GetNumOfSections
	mov ebx, size IMAGE_SECTION_HEADER
	mul ebx
	add ecx, eax
	add ecx, size IMAGE_NT_HEADERS
	mov @correctSize, ecx
	
	mov eax, @correctSize
	
	ret
CalcCorrectPeHeaderSize endp

ReadPeHeaderFromProcess proc hProc:DWORD
  	LOCAL @correctSize: DWORD
  	LOCAL @readSize: DWORD

  	mov @readSize, size IMAGE_DOS_HEADER
  	add @readSize, 300h
  	add @readSize, size IMAGE_NT_HEADERS

  	invoke crt_calloc, 1, @readSize
  	mov g_pHeaderMemory, eax
  
  	invoke ReadMemoryPartlyFromProcess, hProc, g_hModuleBase, @readSize, g_pHeaderMemory
  	.if eax == NULL
  		invoke GetDosAndNtHeader, @readSize
  	
  		invoke CalcCorrectPeHeaderSize
  		mov @correctSize, eax
  	
  		.if @readSize < eax
  			mov @readSize, eax
  			invoke crt_free, g_pHeaderMemory
  			
  			invoke crt_calloc, 1, @readSize
  			mov g_pHeaderMemory, eax
  			invoke ReadMemoryPartlyFromProcess, hProc, g_hModuleBase, @readSize, g_pHeaderMemory
  			invoke GetDosAndNtHeader, @readSize 
  			mov eax, TRUE
  			ret
  		.endif
  	
  	.elseif
		mov eax, TRUE
		ret
  	.endif
  	mov eax, FALSE
  	ret
ReadPeHeaderFromProcess endp

GetSectionHeaders proc uses ecx esi edi
	LOCAL @pSection:DWORD
	LOCAL @NumOfSections:DWORD
	LOCAL @peFileSection:sPeFileSection
	
	invoke RtlZeroMemory, addr @peFileSection, size sPeFileSection
	mov esi, g_pNTHeader
    assume esi:ptr IMAGE_NT_HEADERS
    lea eax, [esi].OptionalHeader
    movzx ecx, [esi].FileHeader.SizeOfOptionalHeader
    add eax, ecx
    mov @pSection, eax
    
    invoke GetNumOfSections
    mov @NumOfSections, eax
    
    invoke crt_calloc, @NumOfSections, size sPeFileSection
    mov g_pListPeSection, eax
    
    xor ecx, ecx
    .while ecx < @NumOfSections
    	push ecx
    	invoke crt_memcpy, addr @peFileSection.sectionHeader, pSection, size IMAGE_SECTION_HEADER
    	pop ecx
    	
    	add pSection, size IMAGE_SECTION_HEADER
    	inc ecx
    .endw
	
	ret

GetSectionHeaders endp

; 初始化进程中的Pe信息
InitPeFromProcess proc hProc:DWORD
	
	.if g_hModuleBase
		invoke ReadPeHeaderFromProcess, hProc
		invoke GetSectionHeaders
	.endif
	
  	ret
InitPeFromProcess endp


ClearDumpCtx proc
  	.if g_pHeaderMemory
    	invoke crt_free, g_pHeaderMemory
    	mov g_pHeaderMemory, 0
  	.endif
  	
  	 .if g_pListPeSection
    	invoke crt_free, g_pListPeSection
    	mov g_pListPeSection, 0
  	.endif
  	ret
ClearDumpCtx endp


DumpProcess proc hProc:DWORD
	LOCAL @hDumpFile:HANDLE
    LOCAL @dwBytesReaded:DWORD
	LOCAL @dwBytesWrited:DWORD
	LOCAL @hModuleNeedByte:DWORD
	LOCAL @pDosHdr:ptr IMAGE_DOS_HEADER
	
	; 获取模块基址
	invoke EnumProcessModules, hProc, offset g_hModuleBase, 4, addr @hModuleNeedByte
	.if g_hModuleBase == NULL
		ret
	.endif
  	invoke InitPeFromProcess, hProc
	
	invoke RtlZeroMemory, offset g_szDumpFileName, 256
    
    invoke GetFileName
    invoke crt_strcpy, offset g_szDumpFileName, eax
    invoke crt_strcat, offset g_szDumpFileName, offset g_szDump
    invoke GetFileExt
    invoke crt_strcat, offset g_szDumpFileName, eax
    
    ;映射dump文件
    invoke CreateFile, offset g_szDumpFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    .if eax == INVALID_HANDLE_VALUE
    	invoke crt_printf, offset g_szFileCreateErrorTip
    	ret
    .endif
    mov @hDumpFile, eax
    
    
    invoke crt_printf, offset g_szDumpDone
	invoke CloseHandle, @hDumpFile
	
	invoke ClearDumpCtx
	ret

DumpProcess endp
	

end