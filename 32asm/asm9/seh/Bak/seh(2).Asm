.586
.model flat,stdcall
option casemap:none

   include windows.inc
   include user32.inc
   include kernel32.inc
   
   includelib user32.lib
   includelib kernel32.lib

assume fs:nothing

.data
    g_szRunMsg db "RunCode", 0
    g_buBuff db 6ah, 00
.code

RunFn proc
	
	invoke MessageBox, NULL, offset g_szRunMsg, NULL, MB_OK
	ret

RunFn endp

DecodeHandler proc pER:ptr EXCEPTION_RECORD, pFrame:LPVOID, pContext:ptr CONTEXT, pDisp:LPVOID
    LOCAL @dwOldProc:DWORD
    mov esi, pER
    assume esi:ptr EXCEPTION_RECORD
    mov edi, pContext
    assume edi:ptr CONTEXT
    
    .if [esi].ExceptionCode == == STATUS_PRIVILEGED_INSTRUCTION and edi.
        invoke VirtualProtect, offset RunFn, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
		mov eax, 0
		.while byte ptr RunFn[eax] != 0c3h
			sub byte ptr RunFn[eax], 3
			inc eax
		.endw
    	invoke VirtualProtect, offset RunFn, 1, @dwOldProc, addr @dwOldProc
    .endif

    
    mov eax, ExceptionContinueExecution
    ret

DecodeHandler endp

EncodeHandler proc
    LOCAL @dwOldProc:DWORD
	invoke VirtualProtect, offset RunFn, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
	mov eax, 0
	.while byte ptr RunFn[eax] != 0c3h
		add byte ptr RunFn[eax], 3
		inc eax
	.endw
    invoke VirtualProtect, offset RunFn, 1, @dwOldProc, addr @dwOldProc
	ret
EncodeHandler endp
start:
     ;注册SEH回调
    push offset DecodeHandler
    push fs:[0]
    mov fs:[0], esp
    
    invoke EncodeHandler
    
    invoke RunFn
    
    
    ;移除SEH回调
    pop fs:[0]
    add esp, 4
    xor eax, eax
    
    
	invoke ExitProcess,eax
	
end start
