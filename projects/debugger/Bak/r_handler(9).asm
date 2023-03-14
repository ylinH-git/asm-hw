.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szEaxToESI	db "EAX=%08X EBX=%08X ECX=%08X EDX=%08X ESI=%08X", 0dh, 0ah, 0
	g_szEdiToFS		db "EDI=%08X ESP=%08X EBP=%08X FS= %08X ", 0dh, 0ah, 0
	g_szCsToEip		db "CS =%08X DS =%08X ES =%08X SS= %08X EIP=%08X", 0dh, 0ah, 0
	g_szFlags  	 	db "CF:%d PF:%d AF:%d ZF:%d SF:%d TF:%d IF:%d DF:%d OF:%d", 0dh, 0ah, 0
.code 

regEAX equ 0
regEBX equ 1
regECX equ 2
regEDX equ 3
regESI equ 4
regEDI equ 5
regESP equ 6
regEBP equ 7
regFS equ 8
regCS equ 9
regDS equ 10
regES equ 11
regSS equ 12
regEIP equ 13
regFLAGS equ 14

HandlerRCChange proc pCtx:DWORD dwReg:DWORD, dwVal:DWORD
	.if dwReg == regEAX
	.elseif dwReg == regEBX
	.elseif dwReg == regECX
	.elseif dwReg == regEDX
	.elseif dwReg == regESI
	.elseif dwReg == regEDI
	.elseif dwReg == regESP
	.elseif dwReg == regEBP
	.elseif dwReg == regFS
	.elseif dwReg == regCS
	.elseif dwReg == regDS
	.elseif dwReg == regES
	.elseif dwReg == regSS
	.elseif dwReg == regEIP	
	.elseif dwReg == regFLAGS							
	.endif
	
	ret

HandlerRCChange endp

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
	mov eax, @ctx.regFlag
	.if eax & 1
		mov @CF, 1
	.elseif
		mov @CF, 0
	.endif
	
	.if eax & 3
		mov @PF, 1
	.elseif
		mov @PF, 0
	.endif
	
	.if eax & 16
		mov @AF, 1
	.elseif
		mov @AF, 0
	.endif
	
	.if eax & 64
		mov @ZF, 1
	.elseif
		mov @ZF, 0
	.endif
	
	.if eax & 128
		mov @SF, 1
	.elseif
		mov @SF, 0
	.endif
	
	.if eax & 256
		mov @TF, 1
	.elseif
		mov @TF, 0
	.endif
	
	.if eax & 512
		mov @IF, 1
	.elseif
		mov @IF, 0
	.endif
	
	.if eax & 1024
		mov @DF, 1
	.elseif
		mov @DF, 0
	.endif
	
	.if eax & 2048
		mov @OF, 1
	.elseif
		mov @OF, 0
	.endif
	invoke crt_printf, offset g_szFlags, @CF, @PF, @AF, @ZF, @SF, @TF, @IF, @DF, @OF
    invoke CloseHandle, @hThread
   
	 
    ret

HandlerRCommand endp


end