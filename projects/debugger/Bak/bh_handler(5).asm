.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	
.code 

SetBhCommand proc dwAddr:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE

    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, g_de.dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    
    mov eax, dwAddr
    mov @ctx.iDr0, eax
    
    
	; Ó²¼þ·ÃÎÊ¶Ïµã
    or @ctx.iDr7, 1;
    and @ctx.iDr7, 0fff0ffffh
    
    invoke SetContext, addr @ctx, @hThread
    invoke CloseHandle, @hThread
   
    
    ret

SetBhCommand endp
end