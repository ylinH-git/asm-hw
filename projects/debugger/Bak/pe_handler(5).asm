.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include disasm.inc
.data 
	g_fileFlag db "rb", 0
	g_peErr db "获取pe数据失败",0dh, 0ah, 0
	g_pFile dd 0
	g_ntOffset dd 0
	g_optionalOffset dd 0
	g_dataDirectoryOffset dd 0
	g_sectionHeaderOffset dd 0;
	g_importDescriptorOffset dd 0
	g_importDescriptorFA dd 0
	
	
	g_dataDirectoryLen dd 0
	g_sectionHeaderLen dd 0
	g_importDescriptorLen dd 0
	
    ; pe结构数据
	; 数据目录数组首地址
    g_dataDirectoris dd 0
    ; 节表数组首地址
   	g_sectionHeaders dd 0
   	; 导入表数组首地址
    g_importDescriptors dd 0
    ; 导入表中dll名称地址数组首地址
    g_importDllNameFAs dd 0
    ; 导入表中每个dll加载的函数长度数组首地址
    g_funcsLens dd 0
   	
   	g_ntHeader IMAGE_NT_HEADERS <?>
	g_fileHeader IMAGE_FILE_HEADER <?>
	g_optionalHeader IMAGE_OPTIONAL_HEADER32 <>
	g_dosHeaderBuf IMAGE_DOS_HEADER <?>
	
	g_szCurrFuncFullName  db 512 dup(0)
	g_szDllName db 256 dup(0)
	g_szFuncName db 256 dup(0)
	g_szPoint db "."
.code 

GetRVAtoFA proc uses esi ecx edi ebx dwRva:DWORD
	LOCAL @offset:DWORD
	LOCAL @dwSectionLen:DWORD
	
	mov eax, g_sectionHeaderLen
	mov @dwSectionLen, eax
	dec @dwSectionLen
	
	xor ecx, ecx
	.while ecx < @dwSectionLen
		mov eax, size IMAGE_SECTION_HEADER
		mul ecx
		add eax, g_sectionHeaders
		mov esi, eax
		assume esi:ptr IMAGE_SECTION_HEADER
		mov eax, [esi].VirtualAddress
		
		mov edi, esi
		add edi, size IMAGE_SECTION_HEADER
		assume edi:ptr IMAGE_SECTION_HEADER
		mov ebx, [edi].VirtualAddress
		.if dwRva >= eax && dwRva < ebx
			mov eax, dwRva
			sub eax, [esi].VirtualAddress
			add eax, [esi].PointerToRawData 
			mov @offset, eax
			.break
		.endif
		
		mov eax, size IMAGE_SECTION_HEADER
		mov ebx, g_sectionHeaderLen
		dec ebx
		mul ebx
		add eax, g_sectionHeaders
		mov esi, eax
		assume esi:ptr IMAGE_SECTION_HEADER
		mov eax, [esi].VirtualAddress
		.if dwRva >= eax
			mov eax, dwRva
			sub eax, [esi].VirtualAddress
			add eax, [esi].PointerToRawData 
			mov @offset, eax
			.break
		.endif
		inc ecx
	.endw
	
	mov eax, @offset
	ret
GetRVAtoFA endp

GetRVAtoVA proc uses esi dwRva:DWORD
	
	mov esi, offset g_optionalHeader
	assume esi: ptr IMAGE_OPTIONAL_HEADER32
	mov eax, dwRva
	add eax, [esi].ImageBase
	ret

GetRVAtoVA endp


GetFAtoRVA proc uses esi ecx edi ebx dwFa:DWORD
	LOCAL @offset:DWORD
	LOCAL @dwSectionLen:DWORD
	
	mov eax, g_sectionHeaderLen
	mov @dwSectionLen, eax
	dec @dwSectionLen
	
	xor ecx, ecx
	.while ecx < @dwSectionLen
		mov eax, size IMAGE_SECTION_HEADER
		mul ecx
		add eax, g_sectionHeaders
		mov esi, eax
		assume esi:ptr IMAGE_SECTION_HEADER
		mov eax, [esi].PointerToRawData
		
		mov edi, esi
		add edi, size IMAGE_SECTION_HEADER
		assume edi:ptr IMAGE_SECTION_HEADER
		mov ebx, [edi].PointerToRawData
		.if dwFa >= eax && dwFa < ebx
			mov eax, dwFa
			sub eax, [esi].PointerToRawData
			add eax, [esi].VirtualAddress;
			mov @offset, eax
			.break
		.endif
		
		mov eax, size IMAGE_SECTION_HEADER
		mov ebx, g_sectionHeaderLen
		dec ebx
		mul ebx
		add eax, g_sectionHeaders
		mov esi, eax
		assume esi:ptr IMAGE_SECTION_HEADER
		mov eax, [esi].PointerToRawData
		.if dwFa >= eax
			mov eax, dwFa
			sub eax, [esi].PointerToRawData
			add eax, [esi].VirtualAddress;
			mov @offset, eax
			.break
		.endif
		inc ecx
	.endw
	
	mov eax, @offset
	ret

GetFAtoRVA endp

GetFAtoVA proc dwFa:DWORD
	
	invoke GetFAtoRVA, dwFa
	invoke GetRVAtoVA, eax
	ret

GetFAtoVA endp

GetVAtoRVA proc uses esi dwVa:DWORD
	
	mov esi, offset g_optionalHeader
	assume esi:ptr IMAGE_OPTIONAL_HEADER
	mov eax, dwVa
	sub eax, [esi].ImageBase
	ret

GetVAtoRVA endp

GetVAtoFA proc dwVa:DWORD
	
	invoke GetVAtoRVA, dwVa
	invoke GetRVAtoFA, eax
	ret

GetVAtoFA endp

GetDosStruct proc uses esi
	invoke RtlZeroMemory, offset g_dosHeaderBuf, size IMAGE_DOS_HEADER
	invoke crt_fseek, g_pFile, 0, SEEK_SET
	invoke crt_fread, offset g_dosHeaderBuf, 1, size IMAGE_DOS_HEADER, g_pFile
	
	mov esi, offset g_dosHeaderBuf
	assume esi:ptr IMAGE_DOS_HEADER
	mov eax, [esi].e_lfanew
	mov g_ntOffset, eax
	
	mov eax, esi
	ret

GetDosStruct endp

GetNtStruct proc
	invoke RtlZeroMemory, offset g_ntHeader, size IMAGE_NT_HEADERS
	invoke crt_fseek, g_pFile, g_ntOffset, SEEK_SET
	invoke crt_fread, offset g_ntHeader, 1, size IMAGE_NT_HEADERS, g_pFile
	
	mov eax, offset g_ntHeader
	ret

GetNtStruct endp

GetFileHStruct proc uses esi ebx
	mov esi, offset g_ntHeader
	assume esi:ptr IMAGE_NT_HEADERS
	lea ebx, [esi].FileHeader
	invoke crt_memcpy, offset g_fileHeader, ebx, size IMAGE_FILE_HEADER
	
	mov eax, offset g_fileHeader
	ret

GetFileHStruct endp

GetOptionalStruct proc uses esi ecx
	mov esi, offset g_ntHeader
	assume esi:ptr IMAGE_NT_HEADERS
	mov eax, g_ntOffset
	add eax, size DWORD
	add eax, size IMAGE_FILE_HEADER
	mov g_optionalOffset, eax
	
	invoke RtlZeroMemory, offset g_optionalHeader, size IMAGE_OPTIONAL_HEADER32
	invoke crt_fseek, g_pFile, g_optionalOffset, SEEK_SET
	invoke crt_fread, offset g_optionalHeader, 1, size IMAGE_OPTIONAL_HEADER32, g_pFile
	
	mov eax, offset g_optionalHeader
	ret
GetOptionalStruct endp

GetDataDirStruct proc uses esi ebx
	mov esi, offset g_optionalHeader
	assume esi:ptr IMAGE_OPTIONAL_HEADER32
	mov eax, [esi].NumberOfRvaAndSizes
	mov g_dataDirectoryLen, eax
	mov eax, g_optionalOffset
	mov g_dataDirectoryOffset, eax
	add g_dataDirectoryOffset, 96
	
	invoke crt_fseek, g_pFile, g_dataDirectoryOffset, SEEK_SET
	invoke crt_calloc, g_dataDirectoryLen, size IMAGE_DATA_DIRECTORY
	mov g_dataDirectoris, eax
	
	mov eax, g_dataDirectoryLen
	mov ebx, size IMAGE_DATA_DIRECTORY
	mul ebx
	mov ebx, eax
	invoke crt_fread, g_dataDirectoris, 1, ebx, g_pFile
	
	mov eax, g_dataDirectoris
	ret

GetDataDirStruct endp

GetSectionHeaders proc uses esi
	mov esi, offset g_fileHeader
	assume esi:ptr IMAGE_FILE_HEADER
	mov eax, g_optionalOffset
	add ax, [esi].SizeOfOptionalHeader
	mov g_sectionHeaderOffset, eax
	
	invoke crt_fseek, g_pFile, g_sectionHeaderOffset, SEEK_SET
	xor eax, eax
	mov ax, [esi].NumberOfSections
	mov g_sectionHeaderLen, eax
	invoke crt_calloc, g_sectionHeaderLen, size IMAGE_SECTION_HEADER
	mov g_sectionHeaders, eax
	
	mov eax, g_sectionHeaderLen
	mov ebx, size IMAGE_SECTION_HEADER
	mul ebx
	mov ebx, eax
	invoke crt_fread, g_sectionHeaders, 1, ebx, g_pFile
	
	mov eax, g_sectionHeaders
	ret

GetSectionHeaders endp

GetImportDirectory proc uses esi ebx edx
	LOCAL @tempImport: DWORD
	LOCAL @allZero: DWORD
	LOCAL @tempOft: DWORD
	
	mov eax, g_dataDirectoris
	add eax, size IMAGE_DATA_DIRECTORY
	mov esi, eax
	assume esi: ptr IMAGE_DATA_DIRECTORY
	
	mov eax, [esi].VirtualAddress
	mov g_importDescriptorOffset, eax
	invoke GetRVAtoFA, g_importDescriptorOffset
	mov g_importDescriptorFA, eax
	mov g_importDescriptorLen, 0
	
	invoke crt_fseek, g_pFile, g_importDescriptorFA, SEEK_SET
	invoke crt_calloc, size IMAGE_IMPORT_DESCRIPTOR, 1
	mov @tempImport, eax
	invoke crt_calloc, size IMAGE_IMPORT_DESCRIPTOR, 1
	mov @allZero, eax
	
	.while TRUE
		invoke crt_fread, @tempImport, 1,  size IMAGE_IMPORT_DESCRIPTOR, g_pFile
		invoke crt_memcmp, @tempImport, @allZero, size IMAGE_IMPORT_DESCRIPTOR
		.break .if eax == 0
		
		inc g_importDescriptorLen
	.endw
	
	invoke crt_calloc, g_importDescriptorLen, size IMAGE_IMPORT_DESCRIPTOR
	mov g_importDescriptors, eax
	invoke crt_calloc, g_importDescriptorLen, size DWORD
	mov g_importDllNameFAs, eax
	invoke crt_calloc, g_importDescriptorLen, size DWORD
	mov g_funcsLens, eax
	
	invoke crt_fseek, g_pFile, g_importDescriptorFA, SEEK_SET
	mov eax, size IMAGE_IMPORT_DESCRIPTOR
	mov ebx, g_importDescriptorLen
	mul ebx
	mov ebx, eax
	invoke crt_fread, g_importDescriptors, 1, ebx, g_pFile
	
	xor ecx, ecx
	.while ecx < g_importDescriptorLen
		mov eax, size DWORD
		mul ecx
		mov ebx, eax
		add ebx, g_importDllNameFAs
		
		mov eax, size IMAGE_IMPORT_DESCRIPTOR
		mul ecx
		mov esi, eax
		add esi, g_importDescriptors
		assume esi:ptr IMAGE_IMPORT_DESCRIPTOR
		mov eax, [esi].Name1
		invoke GetRVAtoFA, eax
		mov dword ptr [ebx], eax
		
		mov eax, [esi].OriginalFirstThunk
		invoke GetRVAtoFA,eax
		mov ebx, eax
		push ecx
		invoke crt_fseek, g_pFile, ebx, SEEK_SET
		pop ecx
		.while TRUE
			push ecx
			invoke crt_fread, addr @tempOft, 1, size DWORD, g_pFile
			pop ecx
			.if @tempOft == 0
				.break
			.endif
			mov eax, size DWORD
			mul ecx
			add eax, g_funcsLens
			inc dword ptr [eax]
		.endw
		inc ecx
	.endw
	
	invoke crt_free, @tempImport
	invoke crt_free, @allZero
	
	mov eax, g_importDescriptors
	ret
GetImportDirectory endp


GetDosStructAddr proc
	mov eax, offset g_dosHeaderBuf
	ret

GetDosStructAddr endp

GetNtStructAddr proc
	mov eax, offset g_ntHeader
	ret

GetNtStructAddr endp

GetFileHStructAddr proc
	mov eax, offset g_fileHeader
	ret

GetFileHStructAddr endp

GetOptionalStructAddr proc
	mov eax, offset g_optionalHeader
	ret
GetOptionalStructAddr endp

GetDataDirStructAddr proc
	mov eax, g_dataDirectoris
	ret
GetDataDirStructAddr endp

GetSectionHeadersAddr proc
	mov eax, g_sectionHeaders
	ret
GetSectionHeadersAddr endp

GetImportDirectoryAddr proc
	mov eax, g_importDescriptors
	ret
GetImportDirectoryAddr endp

GetImportDllNameFAsAddr proc
	mov eax, g_importDllNameFAs
	ret
GetImportDllNameFAsAddr endp

GetDataDirStructLen proc
	mov eax, g_dataDirectoryLen
	ret
GetDataDirStructLen endp

GetSectionHeadersLen proc
	mov eax, g_sectionHeaderLen
	ret
GetSectionHeadersLen endp

GetImportDirectoryLen proc
	mov eax, g_importDescriptorLen
	ret
GetImportDirectoryLen endp

GetPeData proc
	invoke crt_fopen, offset g_szExe, offset g_fileFlag
	mov g_pFile, eax
	.if eax == 0
		invoke crt_printf, offset g_peErr
		mov eax, 0
		ret
	.endif
	
	invoke GetDosStruct
	invoke GetNtStruct
	invoke GetFileHStruct
	invoke GetOptionalStruct
	invoke GetDataDirStruct
	invoke GetSectionHeaders
	invoke GetImportDirectory
	
	ret

GetPeData endp

ClearPeData proc
	invoke crt_fclose, g_pFile
	.if g_dataDirectoris != 0
		invoke crt_free, g_dataDirectoris
		mov g_dataDirectoris, 0
	.endif
	
	.if g_sectionHeaders != 0
		invoke crt_free, g_sectionHeaders
		mov g_sectionHeaders, 0
	.endif
	
	.if g_importDescriptors != 0
		invoke crt_free, g_importDescriptors
		mov g_importDescriptors, 0
	.endif
	
	.if g_importDllNameFAs != 0
		invoke crt_free, g_importDllNameFAs
		mov g_importDllNameFAs, 0
	.endif
	.if g_funcsLens != 0
		invoke crt_free, g_funcsLens
		mov g_funcsLens, 0
	.endif
	ret
ClearPeData endp

FindFuncName proc uses ecx esi edi ebx edx hProc:DWORD, dwFuncAddr:DWORD
	LOCAL @dwFuncRVA:DWORD
	LOCAL @currImport:DWORD
	LOCAL @dwIATStart:DWORD
	LOCAL @bufAsm[64]:BYTE
	LOCAL @bufCode[16]:BYTE
    LOCAL @dwBytesReadWrite:DWORD
    LOCAL @tempAddr:DWORD
    LOCAL @hModule:DWORD
    LOCAL @hModuleNeedByte:DWORD
    LOCAL @dllNameTemp[256]:BYTE 
    LOCAL @funcNameTemp[256]:BYTE 
    LOCAL @nameOffset:DWORD
    LOCAL @wNameRVA:WORD
    LOCAL @needDeep:DWORD
    LOCAL @currImportIdx: DWORD
    LOCAL @currImportFuncsLen: DWORD
    
    mov @needDeep, TRUE
    
    invoke EnumProcessModules, hProc, addr @hModule, 4, addr @hModuleNeedByte
    
    mov @dwIATStart, 0
	invoke GetVAtoRVA, dwFuncAddr
	mov @dwFuncRVA, eax
	xor ecx, ecx
	mov edx, g_importDescriptorLen
	dec edx
	.while ecx < edx
		push edx
		mov eax, size IMAGE_IMPORT_DESCRIPTOR
		mul ecx
		mov esi, eax
		add esi, g_importDescriptors
		assume esi:ptr IMAGE_IMPORT_DESCRIPTOR
		
		mov edi, esi
		add edi, size IMAGE_IMPORT_DESCRIPTOR
		assume edi:ptr IMAGE_IMPORT_DESCRIPTOR
		
		pop edx
		mov eax, [esi].FirstThunk
		mov ebx, [edi].FirstThunk
		.if @dwFuncRVA >= eax && @dwFuncRVA < ebx
			mov ebx, [esi].FirstThunk
			mov @dwIATStart, ebx
			mov @needDeep, FALSE
			mov @currImportIdx, ecx
			.break
		.endif
		
		push edx
		dec edx
		.if ecx == edx
			.if @dwFuncRVA >= ebx
				mov ebx, [edi].FirstThunk
				mov @dwIATStart, ebx
				mov @needDeep, FALSE
				mov @currImportIdx, ecx
				inc @currImportIdx
				.break
			.endif
		.endif
		pop edx
		inc ecx
	.endw
	
	.if @needDeep
		push ecx
    	invoke RtlZeroMemory, addr @bufAsm, 64
    	invoke ReadProcessMemory, hProc, dwFuncAddr, addr @bufCode, 16, addr @dwBytesReadWrite
    	invoke DisasmLine, addr @bufCode, 16, dwFuncAddr, addr @bufAsm
    	pop ecx
		invoke crt_strstr, addr @bufAsm, offset g_szCall
    	.if eax != NULL
    		lea ecx, @bufAsm
    		push ecx
    		invoke crt_strstr, addr @bufAsm, offset g_szPtr
    		pop ecx
    		.if eax != NULL
				add ecx, 16
			.elseif
				add ecx, 5
    		.endif 
    		
        	invoke crt_strtoul, ecx, NULL, 16
        .elseif
        	invoke crt_strstr, addr @bufAsm, offset g_szJmp
			.if eax != NULL
    			lea ecx, @bufAsm
    			push ecx
    			invoke crt_strstr, addr @bufAsm, offset g_szPtr
    			pop ecx
    			.if eax != NULL
					add ecx, 15
				.elseif
					add ecx, 4
    			.endif 
        		invoke crt_strtoul, ecx, NULL, 16
			.endif 
		.endif
		.if eax != 0 && eax != -1
			invoke FindFuncName, hProc, eax
			ret
		.endif		
	.endif
	

	.if @dwIATStart == 0
		mov eax, @dwIATStart
		ret
	.endif

	mov eax, size IMAGE_IMPORT_DESCRIPTOR
	mov ecx, @currImportIdx
	mul ecx
	mov esi, eax
	add esi, g_importDescriptors
	assume esi:ptr IMAGE_IMPORT_DESCRIPTOR
	
	mov eax, size DWORD
	mov ecx, @currImportIdx
	mul ecx
	add eax, g_funcsLens
	mov eax, dword ptr [eax]
	mov @currImportFuncsLen, eax
	
	invoke GetRVAtoVA, @dwIATStart
	mov ecx, eax
	xor edx, edx
	.while edx < @currImportFuncsLen
		push ecx
		invoke RtlZeroMemory,offset g_szCurrFuncFullName, 512
		pop ecx
		.if ecx != NULL
			.if ecx == dwFuncAddr
				mov eax, size DWORD
				mov edx, @currImportFuncsLen
				mul edx
				mov ebx, eax
				add ebx, g_importDllNameFAs
				mov ebx, dword ptr [ebx]
				invoke GetFAtoRVA, ebx
				add eax, @hModule
				mov @tempAddr, eax
				push ecx
				push edx
				invoke ReadProcessMemory, hProc, @tempAddr, addr @dllNameTemp, 256, addr @dwBytesReadWrite
				invoke crt_strcpy, offset g_szDllName, addr @dllNameTemp
				pop edx
				mov eax, size DWORD
				mul edx
				mov ebx, eax
				add ebx, [esi].OriginalFirstThunk
				add ebx, @hModule
				mov @tempAddr, ebx
				invoke ReadProcessMemory, hProc, @tempAddr, addr @nameOffset, 4, addr @dwBytesReadWrite
				.if @nameOffset > 80000000h
					xor eax, eax
					ret
				.endif
				mov ebx, @nameOffset
				xor eax, eax
				mov ax, bx
			
				add eax, @hModule
				add eax, 2
				mov @tempAddr, eax
				push edx
				invoke ReadProcessMemory, hProc, @tempAddr, addr @funcNameTemp, 256, addr @dwBytesReadWrite
				invoke crt_strcpy, offset g_szFuncName, addr @funcNameTemp
				invoke crt_strcat, offset g_szCurrFuncFullName, offset g_szDllName
				invoke crt_strcat, offset g_szCurrFuncFullName, offset g_szPoint
				invoke crt_strcat, offset g_szCurrFuncFullName, offset g_szFuncName
				pop edx
				pop ecx 
				mov eax, offset g_szCurrFuncFullName
				ret
			.endif
		.endif
		add ecx, size DWORD
		inc edx
	.endw
	
	xor eax, eax
	ret

FindFuncName endp
end