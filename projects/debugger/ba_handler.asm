.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
.code 
SetBaCommand proc uses esi dwAddr:DWORD, bIsBaE:DWORD, dwBaLen:DWORD, pDe:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE

	mov esi, pDe
	assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    
    mov eax, dwAddr
    mov @ctx.iDr0, eax
    
    
	; Ó²¼þ¶Ïµã
    or @ctx.iDr7, 000f0001h
    .if bIsBaE == TRUE
    	.if dwBaLen == 1
    		and @ctx.iDr7, 0fff3ffffh
    	.elseif dwBaLen == 2
    		and @ctx.iDr7, 0fff7ffffh
    	.else 
    		and @ctx.iDr7, 0ffffffffh
    	.endif
    .else
    	.if dwBaLen == 1
    		and @ctx.iDr7, 0fff1ffffh
    	.elseif dwBaLen == 2
    		and @ctx.iDr7, 0fff5ffffh
    	.else 
    		and @ctx.iDr7, 0fffdffffh
    	.endif

    .endif

    
    invoke SetContext, addr @ctx, @hThread
    invoke CloseHandle, @hThread
   
    
    ret

SetBaCommand endp
end