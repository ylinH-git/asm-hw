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
	g_szDumpDone db "Dump 完成", 0dh, 0ah,0
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
	g_hProc dd 0

.code

AlignValue proc uses edx ecx badValue:DWORD, alignTo: DWORD 
	LOCAL @temp:DWORD

	mov eax, badValue
	add eax, alignTo
	dec eax
	
	mov ecx, alignTo
	div ecx
	
	mov @temp, eax
	
	mov eax, @temp
	mov ecx, alignTo
	mul ecx
	
	ret
AlignValue endp
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
  	.if eax != NULL
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
  	.endif
  	mov eax, FALSE
  	ret
ReadPeHeaderFromProcess endp

GetSectionHeadersFormProcess proc uses ecx esi edi
	LOCAL @pSection:DWORD
	LOCAL @NumOfSections:DWORD
	
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
    mov edi, g_pListPeSection
    .while ecx < @NumOfSections
    	push ecx
    	invoke crt_memcpy, edi, @pSection, size IMAGE_SECTION_HEADER
    	pop ecx
    	
    	add @pSection, size IMAGE_SECTION_HEADER
    	add edi, size sPeFileSection
    	inc ecx
    .endw
	
	ret

GetSectionHeadersFormProcess endp

; 初始化进程中的Pe信息
InitPeFromProcess proc hProc:DWORD
	
	.if g_hModuleBase
		invoke ReadPeHeaderFromProcess, hProc
		invoke GetSectionHeadersFormProcess
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

ReadPeSectionFromProcess proc uses esi ebx readOffset:DWORD, pPeFileSection:DWORD
	mov esi, pPeFileSection
	assume esi:ptr sPeFileSection
	mov ebx, [esi].dataSize
	invoke crt_calloc, ebx, 1
	mov [esi].data, eax
	
	invoke ReadMemoryPartlyFromProcess, g_hProc, readOffset, [esi].dataSize, [esi].data
	ret
ReadPeSectionFromProcess endp

ReadSectionFrom proc uses esi ecx edx readOffset:DWORD, pPeFileSection:DWORD
	LOCAL @maxReadSize:DWORD
	LOCAL @currentReadSize:DWORD
	LOCAL @pData:DWORD
	LOCAL @valuesFound:DWORD
	LOCAL @readSize:DWORD
	LOCAL @currentOffset:DWORD
	

	mov @maxReadSize, 100
	
	invoke crt_calloc, @maxReadSize, 1
	mov  @pData, eax
	
	mov esi, pPeFileSection
	assume esi:ptr sPeFileSection
	
	mov [esi].data, 0
	mov [esi].dataSize, 0
	mov eax, [esi].normalSize
	mov @readSize, eax
	
	.if readOffset == NULL || @readSize == NULL
		mov eax, TRUE
		invoke crt_free, @pData
		ret
	.endif
	
	mov ecx, @readSize
	.if ecx <= @maxReadSize
		mov [esi].dataSize, ecx
		mov [esi].normalSize, ecx
		
		invoke ReadPeSectionFromProcess, readOffset, pPeFileSection
		invoke crt_free, @pData
		ret
	.endif
	
	mov eax, @readSize
	mov ecx, @maxReadSize
	div ecx
	mov @currentReadSize, edx
	
	.if @currentReadSize == NULL
		mov eax, @maxReadSize
		mov @currentReadSize, eax
	.endif
	
	mov eax, readOffset
	add eax, @readSize
	sub eax, @currentReadSize
	mov @currentOffset, eax
	
	mov ecx, @currentReadSize
	.while ecx >= readOffset
		push ecx
		invoke RtlZeroMemory, @pData, @currentReadSize
		invoke ReadMemoryPartlyFromProcess, g_hProc, @currentOffset, @currentReadSize, @pData
		.break .if eax == NULL
		pop ecx
		
		invoke IsMemoryNotNull,@pData, @currentReadSize
		mov @valuesFound, eax
		.if @valuesFound != NULL
			mov eax, @currentOffset
			add eax, @valuesFound
			mov @currentOffset, eax
			
			.if readOffset < eax
				mov eax, @currentOffset
				sub eax, readOffset
				mov [esi].dataSize, eax
				add [esi].dataSize, size DWORD
				
				mov eax, [esi].normalSize 
				.if eax < [esi].dataSize
					mov [esi].dataSize, eax
				.endif
			
			.endif
			.break
		.endif
		
		
		mov eax, @maxReadSize
		mov @currentReadSize, eax
		
		mov eax, @currentOffset
		sub eax, @currentReadSize
		mov @currentOffset, eax
	.endw
	
	invoke crt_free, @pData
	
	.if [esi].dataSize != NULL
		invoke ReadPeSectionFromProcess,readOffset, esi
	.endif

	ret 
ReadSectionFrom endp

ReadPeSectionsFromProcess proc uses esi ecx
	LOCAL @numOfSection:DWORD
	LOCAL @readOffset:DWORD
	LOCAL @dwRet:DWORD
	mov esi, g_pListPeSection
	assume esi:ptr sPeFileSection
	
	mov @dwRet, TRUE
	invoke GetNumOfSections
	mov @numOfSection, eax
	xor ecx, ecx
	.while ecx < @numOfSection
		mov eax, [esi].sectionHeader.VirtualAddress
		add eax, g_hModuleBase
		mov @readOffset, eax
		
		mov eax, [esi].sectionHeader.Misc.VirtualSize
		mov [esi].normalSize, eax
		push ecx
		invoke crt_printf, offset g_szDumpDone
		pop ecx
		invoke ReadSectionFrom, @readOffset, esi
		.if eax == NULL
			mov @dwRet, FALSE
		.endif
	
		add esi, size sPeFileSection
		inc ecx
	.endw
	mov eax, @dwRet
	ret
ReadPeSectionsFromProcess endp


AlignAllSectionHeaders proc
	
	
	ret

AlignAllSectionHeaders endp


GetSectionHeaderBasedSizeOfImage proc uses esi ecx ebx
	LOCAL @lastVirtualOffset:DWORD
	LOCAL @lastVirtualSize:DWORD
	LOCAL @numOfSection:DWORD
	
	mov esi, g_pListPeSection
	assume esi:ptr sPeFileSection
	
	
	invoke GetNumOfSections
	mov @numOfSection, eax
	xor ecx, ecx
	.while ecx < @numOfSection
		mov eax, [esi].sectionHeader.VirtualAddress
		add eax, [esi].sectionHeader.Misc.VirtualSize
		
		mov ebx, @lastVirtualOffset
		add ebx, @lastVirtualSize
		
		.if eax > ebx
			mov ebx, [esi].sectionHeader.VirtualAddress
			mov @lastVirtualOffset, ebx
			
			mov ebx, [esi].sectionHeader.Misc.VirtualSize
			mov @lastVirtualSize, ebx
		.endif
	
		add esi, size sPeFileSection
		inc ecx
	.endw
	mov eax, @lastVirtualOffset
	add eax, @lastVirtualSize
	ret

GetSectionHeaderBasedSizeOfImage endp

RemoveIatDirectory proc uses esi ecx edx edi
	LOCAL @searchAddress:DWORD
	LOCAL @pDataDirectory:DWORD
	
	mov @searchAddress, 0
	
	mov esi, g_pNTHeader
	assume esi:ptr IMAGE_NT_HEADERS
	
	lea eax, [esi].OptionalHeader.DataDirectory
	mov @pDataDirectory, eax
	
	mov eax, 12
	mov ecx, size IMAGE_DATA_DIRECTORY
	mul eax
	add eax, @pDataDirectory
	
	mov edi, eax
	assume edi:ptr IMAGE_DATA_DIRECTORY
	
	mov eax, [edi].VirtualAddress
	mov @searchAddress, eax
	
	mov [edi].VirtualAddress, 0
	mov [edi].isize, 0
	
	mov esi, g_pListPeSection
	assume esi:ptr sPeFileSection 
	.if @searchAddress
		xor ecx, ecx
		invoke GetNumOfSections
		.while ecx < eax
			mov edx, [esi].sectionHeader.VirtualAddress
			.if edx <= @searchAddress
				mov edx, [esi].sectionHeader.VirtualAddress
				add edx, [esi].sectionHeader.Misc.VirtualSize
				
				.if edx > @searchAddress
					mov edx, [esi].sectionHeader.Characteristics
					or edx, IMAGE_SCN_MEM_READ
					or edx, IMAGE_SCN_MEM_WRITE
					mov [esi].sectionHeader.Characteristics, edx
				.endif
			
			.endif
			
			add esi, size sPeFileSection
			inc ecx
		.endw
	
	.endif
	
	ret

RemoveIatDirectory endp

FixPeHeader proc uses esi ecx ebx edi
	LOCAL @dwSize:DWORD
	LOCAL @pDataDirectory:DWORD
	LOCAL @numberOfRvaAndSizes:DWORD
	
	mov esi, g_pDosHeader
	assume esi:ptr IMAGE_DOS_HEADER
	
	mov eax, [esi].e_lfanew
	add eax, size DWORD
	add eax, size IMAGE_FILE_HEADER
	mov @dwSize, eax
	
	mov edi, g_pNTHeader
	assume edi:ptr IMAGE_NT_HEADERS
	
	lea eax, [edi].OptionalHeader.DataDirectory
	mov @pDataDirectory, eax
	
	mov eax, 11
	mov ecx, size IMAGE_DATA_DIRECTORY
	mul eax
	add eax, @pDataDirectory
	
	mov edi, eax
	assume edi:ptr IMAGE_DATA_DIRECTORY
	mov [edi].VirtualAddress, 0
	mov [edi].isize, 0
	
	mov edi, g_pNTHeader
	assume edi:ptr IMAGE_NT_HEADERS
	mov eax, [edi].OptionalHeader.NumberOfRvaAndSizes
	mov @numberOfRvaAndSizes, eax
	
	mov edi, @pDataDirectory
	assume edi:ptr IMAGE_DATA_DIRECTORY
	xor ecx, ecx
	.while ecx < 16
		.if ecx >= @numberOfRvaAndSizes
			mov [edi].VirtualAddress, 0
			mov [edi].isize, 0
		.endif
		
		add edi, size IMAGE_DATA_DIRECTORY
		inc ecx
	.endw
	
	mov edi, g_pNTHeader
	assume edi:ptr IMAGE_NT_HEADERS 
	
	mov [edi].OptionalHeader.NumberOfRvaAndSizes, 16
	mov [edi].FileHeader.SizeOfOptionalHeader, size IMAGE_OPTIONAL_HEADER
	
	invoke GetSectionHeaderBasedSizeOfImage
	mov [edi].OptionalHeader.SizeOfImage, eax
	
	.if g_hModuleBase
		mov eax, g_hModuleBase
		mov [edi].OptionalHeader.ImageBase, eax
	.endif
	
	mov eax, @dwSize
	add ax, [edi].FileHeader.SizeOfOptionalHeader
	mov esi, eax
		
	invoke GetNumOfSections
	mov ecx, size IMAGE_SECTION_HEADER
	mul ecx
	
	add esi, eax
	
	invoke AlignValue, esi, [edi].OptionalHeader.FileAlignment
	mov [edi].OptionalHeader.SizeOfHeaders, eax
	
	ret
FixPeHeader endp

DumpProcess proc uses esi ecx ebx hProc:DWORD
	LOCAL @hDumpFile:HANDLE
    LOCAL @dwBytesReaded:DWORD
	LOCAL @dwBytesWrited:DWORD
	LOCAL @hModuleNeedByte:DWORD
	LOCAL @pDosHdr:ptr IMAGE_DOS_HEADER
	LOCAL @numOfSection: DWORD
	LOCAL @dwFileOffset:DWORD
	LOCAL @tempData: DWORD
	
	mov @dwFileOffset, 0
	mov @tempData, 0
	
	; 获取模块基址
	invoke EnumProcessModules, hProc, offset g_hModuleBase, 4, addr @hModuleNeedByte
	.if g_hModuleBase == NULL
		ret
	.endif
  	invoke InitPeFromProcess, hProc
  	
  	mov eax, hProc
  	mov g_hProc, eax
  	
  	invoke ReadPeSectionsFromProcess
  	.if eax
  		mov esi, g_pNTHeader
  		assume esi:ptr IMAGE_NT_HEADERS
  		
  		mov [esi].OptionalHeader.FileAlignment, 200h
  		invoke AlignAllSectionHeaders
  		invoke FixPeHeader
  		
  		
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
    	
    	
    	invoke WriteFile, @hDumpFile, g_pDosHeader, size IMAGE_DOS_HEADER, addr @dwBytesWrited, NULL
    	mov eax, @dwBytesWrited
    	add @dwFileOffset, eax
    	invoke WriteFile, @hDumpFile, g_pNTHeader, size IMAGE_NT_HEADERS, addr @dwBytesWrited, NULL
    	mov eax, @dwBytesWrited
    	add @dwFileOffset, eax
    	
    	invoke GetNumOfSections
    	mov @numOfSection, eax
    	xor ecx, ecx
    	mov ebx, g_pListPeSection
    	.while ecx < @numOfSection
    		push ecx
    		invoke WriteFile, @hDumpFile, ebx, size IMAGE_SECTION_HEADER, addr @dwBytesWrited, NULL
    		mov eax, @dwBytesWrited
    		add @dwFileOffset, eax
    		pop ecx
    		
    		add ebx, size sPeFileSection
    		inc ecx
    	.endw
    	
    	
    	xor ecx, ecx
    	mov esi, g_pListPeSection
    	assume esi:ptr sPeFileSection
    	.while ecx < @numOfSection
    		.if [esi].sectionHeader.PointerToRawData == NULL
    			.continue
    		.endif
    		
    		mov eax, @dwFileOffset
    		.if [esi].sectionHeader.PointerToRawData > eax
    			mov eax, [esi].sectionHeader.PointerToRawData
    			sub eax, @dwFileOffset
    			mov @dwBytesWrited, eax
    			add @dwFileOffset, eax
    			
    			push ecx
    			invoke crt_calloc, 1, @dwBytesWrited
    			mov @tempData, eax
    			invoke WriteFile, @hDumpFile, @tempData, @dwBytesWrited, addr @dwBytesWrited, NULL   			
    			invoke crt_free, @tempData
    			pop ecx
    		.endif
    		
    		mov eax, [esi].dataSize
    		mov @dwBytesWrited, eax
    		
    		.if @dwBytesWrited
    			push ecx
    			invoke SetFilePointer ,@hDumpFile, [esi].sectionHeader.PointerToRawData, NULL, FILE_BEGIN
    			invoke WriteFile, @hDumpFile, [esi].data, @dwBytesWrited, addr @dwBytesWrited, NULL
    			mov eax, @dwBytesWrited
    			add @dwFileOffset, eax
    			pop ecx
    			
    			mov eax, [esi].dataSize
    			.if eax < [esi].sectionHeader.SizeOfRawData
    				mov eax, [esi].sectionHeader.SizeOfRawData
    				sub eax, [esi].dataSize
    				mov @dwBytesWrited, eax
    				add @dwFileOffset, eax
    			
    				push ecx
    				invoke crt_calloc, 1, @dwBytesWrited
    				mov @tempData, eax
    				invoke WriteFile, @hDumpFile, @tempData, @dwBytesWrited, addr @dwBytesWrited, NULL   			
    				invoke crt_free, @tempData
    				pop ecx
    		
    			.endif
    		.endif
    		
    		
    		add esi, size sPeFileSection
    		inc ecx
    	.endw
    	
    	
    	invoke crt_printf, offset g_szDumpDone
		invoke CloseHandle, @hDumpFile
  	.endif

	

	invoke ClearDumpCtx
	ret

DumpProcess endp
	

end