.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szBhListHeader	db "序号      断点地址",0dh, 0ah, 0
   	g_szNoneBhList      db "没有硬件断点记录",0dh, 0ah, 0
   	g_szBhListFmt       db "%+4u      %+08X",0dh, 0ah, 0
	g_currBhIdx			dd 0
.code 

ShowBhList proc uses esi pDe:DWORD
	LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE
    LOCAL @dwIsNone: DWORD

	mov @dwIsNone, TRUE
	mov esi, pDe
	assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
	
	invoke crt_printf, offset g_szBhListHeader
	
	.if @ctx.iDr7 & 3
		invoke crt_printf, offset g_szBhListFmt, 0, @ctx.iDr0
		mov @dwIsNone, FALSE
	.endif
	
	.if @ctx.iDr7 & 12
		invoke crt_printf, offset g_szBhListFmt, 1, @ctx.iDr1
		mov @dwIsNone, FALSE
	.endif
	
	.if @ctx.iDr7 & 48
		invoke crt_printf, offset g_szBhListFmt, 2, @ctx.iDr2
		mov @dwIsNone, FALSE
	.endif
	
	.if @ctx.iDr7 & 192
		invoke crt_printf, offset g_szBhListFmt, 3, @ctx.iDr3
		mov @dwIsNone, FALSE
	.endif
	
	.if @dwIsNone
		invoke crt_printf, offset g_szNoneBhList
	.endif
	
	invoke CloseHandle, @hThread
	ret

ShowBhList endp


ClearBhCommand proc uses esi pDe:DWORD,dwIdx:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE

	mov esi, pDe
	assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    
    .if dwIdx == 1
    	and @ctx.iDr7, 0fffffffch
    .endif
    
    .if dwIdx == 2
    	and @ctx.iDr7, 0fffffff3h
    .endif
    
    .if dwIdx == 3
    	and @ctx.iDr7, 0fffffcffh
    .endif
    
    .if dwIdx == 4
    	and @ctx.iDr7, 0fffff3ffh
    .endif
    
    mov eax, dwIdx
    dec eax
    mov g_currBhIdx, eax
    
    invoke SetContext, addr @ctx, @hThread
    invoke CloseHandle, @hThread
   
    
    ret

ClearBhCommand endp


SetBhCommand proc uses esi dwAddr:DWORD, pDe:DWORD, dwLen:DWORD, dwType:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE
	
	mov esi, pDe
	assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    		
    .if @ctx.iDr0 == dwAddr && @ctx.iDr7 & 3
    	ret
    .endif
    
    
    .if @ctx.iDr1 == dwAddr && @ctx.iDr7 & 12
    	ret
    .endif
    
    .if @ctx.iDr2 == dwAddr && @ctx.iDr7 & 48
    	ret
    .endif
    
    
    .if @ctx.iDr3 == dwAddr && @ctx.iDr7 & 192
    	ret
    .endif
    .if g_currBhIdx == 4
    	mov g_currBhIdx, 0
    .endif
    
    .if g_currBhIdx == 0
        mov eax, dwAddr
    	mov @ctx.iDr0, eax

    	or @ctx.iDr7, 3;
    	; e
    	.if dwType == 0
    		and @ctx.iDr7, 0fff0ffffh
    	.endif
    
    	; w
     	.if dwType == 1
     		and @ctx.iDr7, 0fff1ffffh
    	.endif
    
    	; a
     	.if dwType == 2
     		and @ctx.iDr7, 0fff3ffffh
    	.endif
    	
    	.if dwLen == 1
    		or @ctx.iDr7, 00000000h
    	.endif
    
     	.if dwLen == 2
     		or @ctx.iDr7, 00040000h
    	.endif
    
     	.if dwLen == 4
     		or @ctx.iDr7, 000c0000h
    	.endif    	  	
    .endif
    
    .if g_currBhIdx == 1
        mov eax, dwAddr
    	mov @ctx.iDr1, eax

    	or @ctx.iDr7, 12;
    	
    	    	; e
    	.if dwType == 0
    		and @ctx.iDr7, 0ff0fffffh
    	.endif
    
    	; w
     	.if dwType == 1
     		and @ctx.iDr7, 0ff1fffffh
    	.endif
    
    	;a
     	.if dwType == 2
     		and @ctx.iDr7, 0ff3fffffh
    	.endif
    	
    	.if dwLen == 1
    		or @ctx.iDr7, 00000000h
    	.endif
    
     	.if dwLen == 2
     		or @ctx.iDr7, 00400000h
    	.endif
    
     	.if dwLen == 4
     		or @ctx.iDr7, 00c00000h
    	.endif  
    .endif
    
    .if g_currBhIdx == 2
      	mov eax, dwAddr
      	mov @ctx.iDr2, eax
    
      	or @ctx.iDr7, 48;
      	
      	    	; e
    	.if dwType == 0
    		and @ctx.iDr7, 0f0ffffffh
    	.endif
    
    	; w
     	.if dwType == 1
     		and @ctx.iDr7, 0f1ffffffh
    	.endif
    
    	;a
     	.if dwType == 2
     		and @ctx.iDr7, 0f3ffffffh
    	.endif
    	
    	.if dwLen == 1
    		or @ctx.iDr7, 00000000h
    	.endif
    
     	.if dwLen == 2
     		or @ctx.iDr7, 04000000h
    	.endif
    
     	.if dwLen == 4
     		or @ctx.iDr7, 0c000000h
    	.endif  
    .endif
    
    .if g_currBhIdx == 3
      	mov eax, dwAddr
      	mov @ctx.iDr3, eax

      	or @ctx.iDr7, 192;
      	
      	
      	 ; e
    	.if dwType == 0
    		and @ctx.iDr7, 00fffffffh
    	.endif
    
    	; w
     	.if dwType == 1
     		and @ctx.iDr7, 01fffffffh
    	.endif
    
    	;a
     	.if dwType == 2
     		and @ctx.iDr7, 03fffffffh
    	.endif
    	
    	.if dwLen == 1
    		or @ctx.iDr7, 00000000h
    	.endif
    
     	.if dwLen == 2
     		or @ctx.iDr7, 40000000h
    	.endif
    
     	.if dwLen == 4
     		or @ctx.iDr7, 0c0000000h
    	.endif  
    .endif
    
    inc g_currBhIdx
    
    invoke SetContext, addr @ctx, @hThread
    invoke CloseHandle, @hThread
   
    
    ret

SetBhCommand endp

end