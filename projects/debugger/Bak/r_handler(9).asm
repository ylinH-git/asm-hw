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

	mov esi, pDe
	assume esi:ptr DEBUG_EVENT
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, [esi].dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    
    mov esi, addr @ctx
    assume esi:ptr CONTEXT
    invoke crt_printf, offset g_szEaxToESI, [esi].regEax, [esi].regEbx, [esi].regEcx, [esi].regEdx, [esi].regEsi
	invoke crt_printf, offset g_szEdiToFS, [esi].regEdi, [esi].regEsp, [esi].regEbp, [esi].regFs
	invoke crt_printf, offset g_szCsToEip, [esi].regCs, [esi].regDs, [esi].regEs, [esi].regSs, [esi].regEip
	mov eax, [esi].regFlag
	mov @CF, eax
	and @CF, 0000000fh
	invoke crt_printf  
    invoke CloseHandle, @hThread
   
	 
    ret

HandlerRCommand endp


end