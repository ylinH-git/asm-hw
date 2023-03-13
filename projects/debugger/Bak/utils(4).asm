.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include disasm.inc
include pe_handler.inc

.data
	g_szFileName db 256 dup(0)
	g_szFileExt db 256 dup(0)
	g_szFileDrive db 256 dup(0)
	g_szFileDir db 256 dup(0)
.code 
_splitpath PROTO C :DWORD, :DWORD, :DWORD, :DWORD, :DWORD

SplitPath proc pFilePath:DWORD
	invoke _splitpath, pFilePath, offset g_szFileDrive, offset g_szFileDir, offset g_szFileName, offset g_szFileExt
	ret
SplitPath endp

GetFileName proc
	
	mov eax, offset g_szFileName
	ret

GetFileName endp

GetFileExt proc

	mov eax, offset g_szFileExt
	ret

GetFileExt endp

; 获取上下文
GetContext proc pCtx:ptr CONTEXT, hThread:HANDLE
    invoke RtlZeroMemory, pCtx, size CONTEXT
    mov esi, pCtx
    assume esi:ptr CONTEXT
    mov [esi].ContextFlags, CONTEXT_FULL or CONTEXT_DEBUG_REGISTERS
    invoke GetThreadContext, hThread, pCtx
    ret
GetContext endp

; 设置上下文
SetContext proc pCtx:ptr CONTEXT, hThread:HANDLE
    invoke SetThreadContext, hThread, pCtx
    ret
SetContext endp

ReadMemory proc hProc:HANDLE, dwAddr:DWORD, dwSize:DWORD, pBuf:DWORD  
	LOCAL @dwBytesWriteReaded:DWORD
	LOCAL @dwOldProject:DWORD
	
	invoke VirtualProtect, dwAddr, 1, PAGE_EXECUTE_READWRITE,addr @dwOldProject
	invoke ReadProcessMemory, hProc, dwAddr, pBuf, dwSize, addr @dwBytesWriteReaded
	.if eax == NULL
		ret
	.endif
	invoke VirtualProtect, dwAddr, 1, @dwOldProject,addr @dwOldProject
	
	ret

ReadMemory endp

ReadMemoryPartlyFromProcess proc uses esi ecx ebx  hProc:DWORD,dwAddr:DWORD, dwSize:DWORD, pBuf:DWORD 
  LOCAL @addressPart:DWORD
  LOCAL @readBytes:DWORD
  LOCAL @bytesToRead:DWORD
  LOCAL @memBasic: MEMORY_BASIC_INFORMATION

  invoke RtlZeroMemory, addr @memBasic, size MEMORY_BASIC_INFORMATION
  .if hProc == NULL
    mov eax, FALSE
    ret
  .endif

  invoke ReadMemory, hProc, dwAddr, pBuf, dwSize
  .if eax == NULL
    mov eax, dwAddr
    mov @addressPart,  eax
    mov @readBytes, 0
    mov ecx, @readBytes
    .while ecx < dwSize
    	push ecx
   		invoke VirtualQueryEx, hProc, @addressPart, addr @memBasic, size MEMORY_BASIC_INFORMATION
   		pop ecx
    	.if eax == NULL
      		ret
    	.endif
    	
    	mov eax, @memBasic.RegionSize
    	mov @bytesToRead, eax
    	
    	add eax, @readBytes
    	.if eax > dwSize
    		mov eax, dwSize
    		sub eax, @readBytes
    		mov eax @bytesToRead
    	.endif
    	
    	.if @memBasic.State == MEM_COMMIT
    		mov eax, pBuf
    		add eax, @readBytes
    		invoke ReadMemory,hProc, @addressPart, @bytesToRead, eax
    		.if eax == NULL
    			.break
    		.endif
    		
    	.elseif
    		mov ebx, pBuf
    		add ebx, @readBytes
    		invoke RtlZeroMemory, ebx, @bytesToRead
    	.endif
    	
    	mov ecx, @readBytes
    	add ecx, @bytesToRead
    	mov @readBytes, ecx
    	
    	mov eax, @memBasic.RegionSize
    	add eax, @addressPart
    	mov @addressPart, eax
    .endw

	
  .elseif
  	mov eax, TRUE
  .endif

  ret
ReadMemoryPartlyFromProcess endp

WriteMemory proc hProc:HANDLE, destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD
	LOCAL @dwBytesWriteReaded:DWORD
	LOCAL @dwOldProject:DWORD
	
	invoke VirtualProtect, destAddr, 1, PAGE_EXECUTE_READWRITE,addr @dwOldProject
	invoke WriteProcessMemory, hProc, destAddr, sourceAddr, memSize, addr @dwBytesWriteReaded
	invoke VirtualProtect, destAddr, 1, @dwOldProject,addr @dwOldProject
	
	ret

WriteMemory endp


GetCurrEip proc uses esi pDe:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE
    
    mov esi, pDe
    assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS, FALSE, [esi].dwThreadId
    mov @hThread, eax
    invoke GetContext,addr @ctx, @hThread

    invoke CloseHandle, @hThread
    mov eax, @ctx.regEip
    ret

GetCurrEip endp

PrintFunc proc uses ecx hProc:DWORD,pAsmBuf:DWORD
	invoke crt_strstr, pAsmBuf, offset g_szCall
    .if eax != NULL
    	mov ecx, pAsmBuf
    	push ecx
    	invoke crt_strstr, pAsmBuf, offset g_szPtr
    	pop ecx
    	.if eax != NULL
			add ecx, 16
		.elseif
			add ecx, 5
    	.endif 
        invoke crt_strtoul, ecx, NULL, 16
        invoke FindFuncName, hProc, eax
    	ret
	.endif
	
	invoke crt_strstr, pAsmBuf, offset g_szJmp
	.if eax != NULL
    	mov ecx, pAsmBuf
    	push ecx
    	invoke crt_strstr, pAsmBuf, offset g_szPtr
    	pop ecx
    	.if eax != NULL
			add ecx, 15
		.elseif
			add ecx, 4
    	.endif 
        invoke crt_strtoul, ecx, NULL, 16
        invoke FindFuncName, hProc, eax
    	ret
	.endif
	
	xor eax, eax
	ret
PrintFunc endp

GetAsm proc uses ebx hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD
    LOCAL @bufCode[16]:BYTE
    LOCAL @dwBytesReadWrite:DWORD
   
    invoke ReadProcessMemory, hProc, currDwEip, addr @bufCode, 16, addr @dwBytesReadWrite
    invoke DisasmLine, addr @bufCode, 16, currDwEip, pCurBufAsm
    mov ebx, pDwCodeLen
    mov [ebx], eax
    ret
GetAsm endp

PrintAsm proc uses ecx ebx hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD, asmNum: DWORD
	LOCAL @bufAsm[64]:BYTE
    LOCAL @bufCode[16]:BYTE
    LOCAL @dwEip:DWORD
    LOCAL @dwBytesReadWrite:DWORD
    LOCAL @dwLastCodeLen:DWORD
    LOCAL @needSearchFuncName:DWORD
    LOCAL @funcFullNameAddr:DWORD
    
    mov eax, currDwEip
    mov @dwEip, eax
	xor ecx, ecx
    .while ecx < asmNum
    	push ecx
    	invoke RtlZeroMemory, addr @bufAsm, 64
    	invoke ReadProcessMemory, hProc, @dwEip, addr @bufCode, 16, addr @dwBytesReadWrite
    	invoke DisasmLine, addr @bufCode, 16, @dwEip, addr @bufAsm
    	pop ecx
    	mov @dwLastCodeLen, eax
    	.if ecx == 0
    		mov ebx, pDwCodeLen
    		mov [ebx], eax

    		invoke PrintFunc, hProc,addr @bufAsm
    		mov @funcFullNameAddr, eax
    		
    		.if eax != 0
    			push ecx
    			invoke crt_strcpy, pCurBufAsm, addr @bufAsm
    			invoke crt_printf, offset g_szCurFuncAsmFmt, currDwEip, pCurBufAsm, @funcFullNameAddr	
    			pop ecx
    		.elseif
    			push ecx
    			invoke crt_strcpy, pCurBufAsm, addr @bufAsm
    			invoke crt_printf, offset g_szCurAsmFmt, currDwEip, pCurBufAsm	
    			pop ecx
    		.endif

    	.else
    		invoke PrintFunc, hProc,addr @bufAsm
    		mov @funcFullNameAddr, eax
    		.if eax != 0
    			push ecx
    			invoke crt_printf, offset g_szFuncAsmFmt, @dwEip, addr @bufAsm, @funcFullNameAddr
    			pop ecx
    		.elseif
    		    push ecx
    			invoke crt_printf, offset g_szAsmFmt, @dwEip, addr @bufAsm	
    			pop ecx
    		.endif
    	.endif
    	mov eax, @dwLastCodeLen
    	add @dwEip, eax
    	inc ecx
    .endw
   
	ret

PrintAsm endp

DecEip proc uses esi pDe:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:DWORD
    
    mov esi, pDe
    assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    invoke GetContext,addr @ctx, @hThread
   	dec @ctx.regEip
   	invoke SetContext,addr @ctx, @hThread
	invoke CloseHandle, @hThread
	ret

DecEip endp

SetTF proc uses esi pDe:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:DWORD
    
    mov esi, pDe
    assume esi:ptr DEBUG_EVENT    
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    invoke GetContext,addr @ctx, @hThread
   	or @ctx.regFlag, 100h
   	invoke SetContext,addr @ctx, @hThread
	invoke CloseHandle, @hThread
  
	ret

SetTF endp
end