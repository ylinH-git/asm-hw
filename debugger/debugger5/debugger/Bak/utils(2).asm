.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc

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
end