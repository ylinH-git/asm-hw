.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include disasm.inc
include pe_handler.inc

.code 
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
    	add ecx, 5
        invoke crt_strtoul, ecx, NULL, 16
        invoke FindFuncName, eax
    	ret
	.endif
	
	invoke crt_strstr, pAsmBuf, offset g_szJmp
	.if eax != NULL
    	mov ecx, pAsmBuf
    	add ecx, 4
        invoke crt_strtoul, ecx, NULL, 16
        invoke FindFuncName, eax
    	ret
	.endif
	
	xor eax, eax
	ret
PrintFunc endp

PrintAsm proc uses ecx ebx hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD, asmNum: DWORD
	LOCAL @bufAsm[64]:BYTE
    LOCAL @bufCode[16]:BYTE
    LOCAL @dwEip:DWORD
    LOCAL @dwBytesReadWrite:DWORD
    LOCAL @dwLastCodeLen:DWORD
    LOCAL @needSearchFuncName:DWORD
    
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

    		invoke PrintFunc, addr @bufAsm
    		push ecx
    		.if eax != 0
    			invoke crt_strcpy, pCurBufAsm, addr @bufAsm
    			invoke crt_printf, offset g_szCurFuncAsmFmt, currDwEip, pCurBufAsm, pCurBufAsm	
    		.elseif
    			invoke crt_strcpy, pCurBufAsm, addr @bufAsm
    			invoke crt_printf, offset g_szCurAsmFmt, currDwEip, pCurBufAsm	
    		.endif
    		pop ecx
    	.else
    		invoke PrintFunc, addr @bufAsm
    		push ecx
    		.if eax != 0
    			invoke crt_printf, offset g_szFuncAsmFmt, @dwEip, addr @bufAsm, addr @bufAsm
    			pop ecx
    		.elseif
    			invoke crt_printf, offset g_szAsmFmt, @dwEip, addr @bufAsm	
    		.endif
    		pop ecx
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