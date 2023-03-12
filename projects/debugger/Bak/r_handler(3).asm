.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szEaxToESI	db "EAX=%08X EBX=%08X ECX=%08X EDX=%08X ESI=%08X", 0dh, 0ah, 0
	g_szEdiToFS		db "EDI=%08X ESP=%08X EBP=%08X FS=%08X ", 0dh, 0ah, 0
	g_szCsToEip		db "CS =%08X DS =%08X ES =%08X SS=%08X EIP=%08X", 0dh, 0ah, 0
	g_szFlags  	 	db "CF:%d PF:%d AF:%d ZF:%d SF:%d TF:%d IF:%d DF:%d OF:%d", 0dh, 0ah, 0
.code 

HandlerRCommand proc uses esi pDe:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE
    LOCAL @CF:DWORD
    LOCAL @PF:DWORD
    LOCAL @AF:DWORD
    LOCAL @ZF:DWORD
   	LOCAL @SF:DWORD
   	LOCAL @TF:DWORD
   	LOCAL @IF:DWORD
   	LOCAL @DF:DWORD
   	LOCAL @OF:DWORD

	mov esi, pDe
	assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    
    invoke crt_printf, offset g_szEaxToESI, @ctx.regEax, @ctx.regEbx, @ctx.regEcx, @ctx.regEdx, @ctx.regEsi
	invoke crt_printf, offset g_szEdiToFS, @ctx.regEdi, @ctx.regEsp, @ctx.regEbp, @ctx.regFs
	invoke crt_printf, offset g_szCsToEip, @ctx.regCs, @ctx.regDs, @ctx.regEs, @ctx.regSs, @ctx.regEip
	mov eax, [esi].regFlag
	.if eax & 1
		mov @CF, 1
	.elseif
		mov @CF, 0
	.endif
	
	.if eax & 1
		mov @PF, 1
	.elseif
		mov @PF, 0
	.endif
	
	.if eax & 1
		mov @AF, 1
	.elseif
		mov @AF, 0
	.endif
	
	.if eax & 1
		mov @ZF, 1
	.elseif
		mov @ZF, 0
	.endif
	
	.if eax & 1
		mov @SF, 1
	.elseif
		mov @SF, 0
	.endif
	
	.if eax & 1
		mov @TF, 1
	.elseif
		mov @TF, 0
	.endif
	
	.if eax & 1
		mov @IF, 1
	.elseif
		mov @IF, 0
	.endif
	
	.if eax & 1
		mov @DF, 1
	.elseif
		mov @DF, 0
	.endif
	
	.if eax & 1
		mov @OF, 1
	.elseif
		mov @OF, 0
	.endif
	invoke crt_printf, offset g_szFlags, @CF, @PF, @AF, @ZF, @SF, @TF, @IF, @DF, @OF
    invoke CloseHandle, @hThread
   
	 
    ret

HandlerRCommand endp


end