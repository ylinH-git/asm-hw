.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include disasm.inc
include utils.inc

.const
	g_szExe 						db "test.exe", 0
	g_szSystemBegin 				db "start debugging...", 0dh, 0ah, 0
    g_szOUTPUT_DEBUG_STRING_EVENT  	db "OUTPUT_DEBUG_STRING_EVENT",0
    g_szCommandErrTip              	db "命令出错", 0dh, 0ah, 0
    g_szBpListHeader               	db "序号      断点地址",0dh, 0ah, 0
   	g_szNoneBpList           		db "没有断点记录",0dh, 0ah, 0
   	g_szSystemBp					db "命中系统断点:0x", 0
   	g_szCustomBp					db "命中一般断点:0x", 0
   	g_szBhTip                       db "命中硬件执行断点 %08X", 0dh, 0ah, 0
   	g_szBaETip                      db "命中硬件访问断点 %08X", 0dh, 0ah, 0
   	g_szBaWTip                      db "命中硬件写入断点 %08X", 0dh, 0ah, 0
   	g_szBmTip                      	db "命中内存写入断点 %08X", 0dh, 0ah, 0
    g_szFmt                        	db "%s",0dh, 0ah, 0
    g_szBpFmt                       db "%s%+08X",0dh, 0ah, 0
    g_szBpListFmt                  	db "%+4u      %+08X",0dh, 0ah, 0
    g_szAsmFmt                      db "0x%08X %s",0dh, 0ah, 0
    g_szCurAsmFmt                   db "0x%08X %s <==== ",0dh, 0ah, 0
    g_szCall    					db "call", 0

.data
    g_de        DEBUG_EVENT <0>
    public g_hProc     HANDLE 0
    g_szCommand db 256 dup(0)
    g_bIsSysBp  db TRUE
	g_bpList bpStruct 100 dup(<?>) ;断点数组
	g_bpsLen dd 0 ;断点数组长度
	g_curExcptAddr dd 0
	g_bIsSingleTStep dd FALSE
	g_bIsSinglePStep dd FALSE
 	g_pTFBpOldCode dd 0
	g_dwTFBpAddr dd 0
	
	g_dwTraceEnd dd 0
	
	g_bIsBhSingleStep dd FALSE  ;硬件执行断点的单步标志
	g_bIsBaESingleStep dd FALSE	; 硬件访问断点标志
	g_bIsBaWSingleStep dd FALSE	; 硬件写入断点标志
	g_bIsHardSingleStep dd FALSE; 硬件断点标志
	g_bIsBmSingleStep dd FALSE  ;内存断点的单步标志 
	
	g_dwMmbpAddr dd 0 ;内存断点地址
    g_dwMmbpLen  dd 0 ;内存断点长度 
    g_dwMmbpOld  dd 0 ;原来的内存保护属性
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

OutputDebugHandler proc uses ebx
	LOCAL @szBuf[MAX_PATH]:BYTE
    LOCAL @szBufW[MAX_PATH]:WORD
    LOCAL @dwBytesReaded:DWORD
    LOCAL @dwBufAddr: DWORD
    
    
    invoke crt_printf, offset g_szFmt, offset g_szOUTPUT_DEBUG_STRING_EVENT
    
    invoke RtlZeroMemory,addr @szBuf, MAX_PATH
    
    lea esi, g_de.u.DebugString
    assume esi:ptr OUTPUT_DEBUG_STRING_INFO
    
    xor ebx, ebx
    mov bx, [esi].nDebugStringiLength
    inc ebx
    invoke crt_malloc, ebx
    mov @dwBufAddr, eax
    invoke RtlZeroMemory,@dwBufAddr, ebx
    invoke ReadProcessMemory, g_hProc, [esi].lpDebugStringData,  @dwBufAddr, ebx, addr @dwBytesReaded
    .if !eax
         ret
    .endif
    ; 过去，操作系统不通过 OutputDebugStringW 返回 Unicode 字符串，而是 返回 ASCII 字符串
    ; 若要强制 OutputDebugStringW 返回 Unicode 字符串，调试器需要调用 WaitForDebugEventEx 函数以选择加入新行为。 
    ; 这样，操作系统就知道调试器支持 Unicode，并专门选择接收 Unicode 字符串。
    ; 所以暂时只会走多字节的分支
    .if [esi].fUnicode
        invoke crt_wprintf, offset g_szFmt, @dwBufAddr
     .else
       	invoke crt_printf, offset g_szFmt, @dwBufAddr
    .endif

	invoke crt_free, @dwBufAddr
	ret

OutputDebugHandler endp
WriteMemory proto destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD


GetCurrEip proc
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE
    
    invoke OpenThread,THREAD_ALL_ACCESS, FALSE, g_de.dwThreadId
    mov @hThread, eax
    invoke GetContext,addr @ctx, @hThread

    invoke CloseHandle, @hThread
    mov eax, @ctx.regEip
    ret

GetCurrEip endp

ClearBpList proc uses ecx ebx edx dwAddr:DWORD
	mov ebx, offset g_bpList
	assume ebx:ptr bpStruct
	
	.if dwAddr == 0 || dwAddr == -1
		xor ecx, ecx
		.while ecx < g_bpsLen
			push ecx
			lea ecx, [ebx].m_oldCode
    		;还原指令
    		invoke WriteMemory, [ebx].m_dwAddr, ecx, 1
			pop ecx
			add ebx, size bpStruct
			inc ecx
		.endw
	
		mov g_bpsLen, 0
		invoke RtlZeroMemory,offset g_bpList, 500

	.elseif
		xor ecx, ecx
		.while ecx < g_bpsLen
			mov edx, dwAddr
			.if [ebx].m_dwAddr == edx
				push ecx
				lea ecx, [ebx].m_oldCode
    			;还原指令
    			invoke WriteMemory, [ebx].m_dwAddr, ecx, 1
				pop ecx
				invoke RtlZeroMemory, ebx, 5
				dec g_bpsLen
				jmp Arrange
			.endif
			add ebx, size bpStruct
			inc ecx
		.endw
Arrange:		
		.while ecx < g_bpsLen
			mov edx, ebx
			add edx, 5
			push ecx
			invoke RtlMoveMemory, ebx, edx, 5
			pop ecx
			inc ecx
		.endw
    .endif

	ret

ClearBpList endp

ShowBpList proc uses ecx ebx edx
	.if g_bpsLen == 0
		invoke crt_printf, offset g_szNoneBpList
		ret
	.endif
	mov ebx, offset g_bpList
	assume ebx:ptr bpStruct
	invoke crt_printf, offset g_szBpListHeader
	xor ecx, ecx
	.while ecx < g_bpsLen
		push ecx
		invoke crt_printf, offset g_szBpListFmt, ecx, [ebx].m_dwAddr
		pop ecx
		add ebx, size bpStruct
		inc ecx
	.endw
	ret

ShowBpList endp

FindBp proc uses ecx ebx dwAddr:DWORD
	mov eax, offset g_bpList
	assume eax:ptr bpStruct
	xor ecx, ecx
	.while ecx < g_bpsLen
		mov ebx, [eax].m_dwAddr
		.if dwAddr == ebx
			ret
		.endif
		add eax, size bpStruct
		inc ecx
	.endw
	mov eax, 0
	ret

FindBp endp

WriteMemory proc destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD
	LOCAL @dwBytesWriteReaded:DWORD
	LOCAL @dwOldProject:DWORD
	
	invoke VirtualProtect, destAddr, 1, PAGE_EXECUTE_READWRITE,addr @dwOldProject
	invoke WriteProcessMemory, g_hProc, destAddr, sourceAddr, memSize, addr @dwBytesWriteReaded
	invoke VirtualProtect, destAddr, 1, @dwOldProject,addr @dwOldProject
	
	ret

WriteMemory endp

PrintAsm proc uses ecx pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD, asmNum: DWORD
	LOCAL @bufAsm[64]:BYTE
    LOCAL @bufCode[16]:BYTE
    LOCAL @dwEip:DWORD
    LOCAL @dwBytesReadWrite:DWORD
    LOCAL @dwLastCodeLen:DWORD
    
    mov eax, currDwEip
    mov @dwEip, eax
	xor ecx, ecx
    .while ecx < asmNum
    	push ecx
    	invoke RtlZeroMemory, addr @bufAsm, 64
    	invoke ReadProcessMemory, g_hProc, @dwEip, addr @bufCode, 16, addr @dwBytesReadWrite
    	invoke DisasmLine, addr @bufCode, 16, @dwEip, addr @bufAsm
    	pop ecx
    	mov @dwLastCodeLen, eax
    	.if ecx == 0
    		mov [pDwCodeLen], eax
    		push ecx
    		invoke crt_strcpy, pCurBufAsm, addr @bufAsm
    		invoke crt_printf, offset g_szCurAsmFmt, currDwEip, pCurBufAsm	
    		pop ecx
    	.else
    		push ecx
    		invoke crt_printf, offset g_szAsmFmt, @dwEip, addr @bufAsm	
    		pop ecx
    	.endif
    	mov eax, @dwLastCodeLen
    	add @dwEip, eax
    	inc ecx
    .endw
   
	ret

PrintAsm endp

DecEip proc
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:DWORD
    
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, g_de.dwThreadId
    mov @hThread, eax
    invoke GetContext,addr @ctx, @hThread
   	dec @ctx.regEip
   	invoke SetContext,addr @ctx, @hThread
	invoke CloseHandle, @hThread
	ret

DecEip endp

SetTF proc
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:DWORD
        
    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, g_de.dwThreadId
    mov @hThread, eax
    invoke GetContext,addr @ctx, @hThread
   	or @ctx.regFlag, 100h
   	invoke SetContext,addr @ctx, @hThread
	invoke CloseHandle, @hThread
  
	ret

SetTF endp

SetBp proc uses ecx ebx edx dwAddr:DWORD, dwFlag:DWORD
    LOCAL @dwBytesWriteReaded:DWORD
    LOCAL @btCC:BYTE
    
    .if dwFlag == 0
    	invoke FindBp, dwAddr
    	.if eax != 0
    		ret
    	.endif
    
    	;保存原指令
    	mov ecx, offset g_bpList
    	mov eax, g_bpsLen
    	mov ebx, size bpStruct
    	mul ebx
    	add ecx, eax
		assume ecx:ptr bpStruct
	
		lea edx, [ecx].m_oldCode
		push ecx
    	invoke ReadProcessMemory, g_hProc, dwAddr, edx, 1, addr @dwBytesWriteReaded
    	pop ecx
    	mov edx, dwAddr
    	mov [ecx].m_dwAddr, edx
    	inc g_bpsLen
    .endif
    	;写入CC
    	mov @btCC, 0cch
    	invoke WriteMemory, dwAddr, addr @btCC, size @btCC
    
    ret

SetBp endp

SetTFBp proc uses ecx ebx edx dwAddr:DWORD
    LOCAL @dwBytesWriteReaded:DWORD
    LOCAL @btCC:BYTE
    invoke ReadProcessMemory, g_hProc, dwAddr, offset g_pTFBpOldCode, 1, addr @dwBytesWriteReaded
    
    ;写入CC
    mov @btCC, 0cch
    invoke WriteMemory, dwAddr, addr @btCC, size @btCC
    
    ret

SetTFBp endp

RestoreBp proc uses ebx ecx dwAddr:DWORD
    invoke FindBp, dwAddr
    .if eax == 0
    	ret
    .endif
    
    mov ebx, eax
    assume ebx:ptr bpStruct
    
    lea ecx, [ebx].m_oldCode
    ;还原指令
    invoke WriteMemory, dwAddr, ecx, 1
    invoke SetTF
    invoke DecEip
    ret

RestoreBp endp

RestoreTFBp proc uses ebx ecx
	invoke FindBp, g_dwTFBpAddr
    .if eax != 0
    	mov ebx, eax
    	assume ebx:ptr bpStruct
    
    	lea ecx, [ebx].m_oldCode
    	;还原指令
    	invoke WriteMemory, g_dwTFBpAddr, ecx, 1
    	invoke SetTF
    .else
    	;还原指令
    	invoke WriteMemory, g_dwTFBpAddr, offset g_pTFBpOldCode , 1
    .endif
    invoke DecEip
    mov g_bIsSinglePStep, FALSE
    ret

RestoreTFBp endp

SetTCommand proc
  	mov g_bIsSingleTStep, TRUE
  	invoke SetTF
   	ret
SetTCommand endp



SetBaCommand proc dwAddr:DWORD, bIsBaE:DWORD, dwBaLen:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE

    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, g_de.dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    
    mov eax, dwAddr
    mov @ctx.iDr0, eax
    
    
	; 硬件断点
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


SetBhCommand proc dwAddr:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE

    invoke OpenThread,THREAD_ALL_ACCESS,FALSE, g_de.dwThreadId
    mov @hThread, eax
    
    invoke GetContext, addr @ctx, @hThread
    
    mov eax, dwAddr
    mov @ctx.iDr0, eax
    
    
	; 硬件访问断点
    or @ctx.iDr7, 1;
    and @ctx.iDr7, 0fff0ffffh
    
    invoke SetContext, addr @ctx, @hThread
    invoke CloseHandle, @hThread
   
    
    ret

SetBhCommand endp

SetBmCommand proc
    
    ;修改内存属性为不可访问
    invoke VirtualProtectEx, g_hProc, g_dwMmbpAddr, g_dwMmbpLen, PAGE_NOACCESS, offset g_dwMmbpOld
    
    ret

SetBmCommand endp

InputCommand proc uses ebx ecx
    LOCAL @bufAsm[64]:BYTE
    LOCAL @bufCode[16]:BYTE
    LOCAL @curBufAsm[64]:BYTE
    LOCAL @currDwEip:DWORD
    LOCAL @dwEip:DWORD
    LOCAL @dwBytesReadWrite:DWORD
    LOCAL @dwCodeLen:DWORD
    LOCAL @dwLastCodeLen:DWORD
    LOCAL @bIsBaE:DWORD
    LOCAL @pAddr:DWORD
    LOCAL @dwBaLen:DWORD
    
    invoke RtlZeroMemory, addr @curBufAsm, 64
    ;输入命令之前，显示五条反汇编
    invoke GetCurrEip
    mov @currDwEip, eax
    
    ;判断是否开启了trace
    mov eax, g_dwTraceEnd
    .if eax != 0
    	push eax
    	invoke PrintAsm, addr @curBufAsm, @currDwEip, addr @dwCodeLen, 1
    	pop eax
        ;判断trace是否结束
        .if eax != @currDwEip
            ;trace没有结束
            invoke SetTCommand
            ret
        .else
            ;trace结束
            mov g_dwTraceEnd, 0
        .endif
    .else
    	invoke PrintAsm, addr @curBufAsm, @currDwEip, addr @dwCodeLen, 5
    .endif
    
    
    
    .while TRUE 
    	invoke RtlZeroMemory, offset g_szCommand, 256
        ;接受命令
        invoke crt_gets, offset g_szCommand
        
        ;解析命令码
        .if g_szCommand[0] == 'b' && g_szCommand[1] == 'p'
            ;解析参数 - 地址（字符串转数字）
            invoke crt_strtoul, offset g_szCommand + 3, NULL, 16
            .if eax == 0 || eax == -1
                ;命令出错，重新输入
                invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            
            invoke SetBp, eax, 0
        .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'l'
        	invoke ShowBpList
        .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'c'
        	invoke crt_strtoul, offset g_szCommand + 3, NULL, 16
        	invoke ClearBpList, eax
        .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'a'
			mov g_bIsBhSingleStep, FALSE
            mov al, g_szCommand[3]
            .if al == 'e'
            	mov g_bIsBaESingleStep, TRUE
            	mov @bIsBaE, TRUE
            .elseif al == 'w'
            	mov g_bIsBaWSingleStep, TRUE
            	mov @bIsBaE, FALSE
            .else
            	invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            invoke crt_strtoul, offset g_szCommand + 5, NULL, 16
            .if eax == 0 || eax == -1
                ;命令出错，重新输入
                invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            mov @dwBaLen, eax
            
            .if @dwBaLen != 1 && @dwBaLen != 2 && @dwBaLen != 4
    			;命令出错，重新输入
				invoke crt_printf, offset g_szCommandErrTip
				.continue
			.endif
            
            invoke crt_strtoul, offset g_szCommand + 7, NULL, 16

            .if eax == 0 || eax == -1
                ;命令出错，重新输入
                invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            mov @pAddr, eax
            invoke SetBaCommand, @pAddr, @bIsBaE, @dwBaLen
            
         .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'h'
        	mov g_bIsBhSingleStep, TRUE
        	mov g_bIsBaESingleStep, FALSE
        	mov g_bIsBaWSingleStep, FALSE
        	invoke crt_strtoul, offset g_szCommand+3, NULL, 16
            .if eax == 0 || eax == -1
                ;命令出错，重新输入
                invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            
            ;设置断点
            invoke SetBhCommand, eax
       	.elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'm'
            invoke crt_strtoul, offset g_szCommand + 3, NULL, 16
            .if eax == 0 || eax == -1
                invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            mov g_dwMmbpAddr, eax
            
            ;解析参数 - 长度
            invoke crt_strtoul, offset g_szCommand + 12, NULL, 16
            .if eax == 0 || eax == -1
                invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            mov g_dwMmbpLen, eax
            
            invoke SetBmCommand
            
       	.elseif g_szCommand[0] == 't' && g_szCommand[1] == 'r'&& g_szCommand[2] == 'a'&& g_szCommand[3] == 'c'&& g_szCommand[4] == 'e'
            invoke crt_strtoul, offset g_szCommand + 6, NULL, 16
            .if eax == 0 || eax == -1
                invoke crt_printf, offset g_szCommandErrTip
                .continue
            .endif
            
            ;启动单步
            mov g_dwTraceEnd, eax
            invoke SetTCommand
            
            ret 
        .elseif g_szCommand[0] == 't'
            invoke SetTCommand
            ret
        .elseif g_szCommand[0] == 'p'
        	invoke crt_strstr, addr @curBufAsm, offset g_szCall
        	 .if eax == NULL
        	 	invoke SetTCommand
            .else
            	mov g_bIsSinglePStep, TRUE
                mov eax, @dwCodeLen
            	add @currDwEip, eax
            	mov eax, @currDwEip
            	mov g_dwTFBpAddr, eax
            	invoke SetTFBp, @currDwEip
            .endif

            ret
        .elseif g_szCommand[0] == 'u'
        	invoke crt_strtoul, offset g_szCommand + 3, NULL, 16
            .if eax != 0 && eax != -1
             	mov @currDwEip, eax
            .endif
            invoke PrintAsm, addr @curBufAsm, @currDwEip, addr @dwCodeLen,5
  
        .elseif g_szCommand[0] == 'g'
            ret
        .endif
    .endw

    ret

InputCommand endp


OnExcept proc
    LOCAL @dwStatus:DWORD
    LOCAL @bIsInputCmd:DWORD
    LOCAL @hThread:HANDLE
	LOCAL @ctx:CONTEXT
	LOCAL @dwOldProc:DWORD

    lea esi, g_de.u.Exception.pExceptionRecord
    assume esi:ptr EXCEPTION_RECORD
    
    
    mov @dwStatus, DBG_CONTINUE
    mov @bIsInputCmd, FALSE
    
    .if [esi].ExceptionCode == EXCEPTION_BREAKPOINT
        ;判断是否是系统断点
        .if g_bIsSysBp
        	invoke crt_printf, offset g_szBpFmt, offset g_szSystemBp, [esi].ExceptionAddress
            mov g_bIsSysBp, FALSE
        .elseif g_bIsSinglePStep == TRUE
        	invoke RestoreTFBp
        	mov eax, g_dwTFBpAddr
        	mov g_curExcptAddr, eax
        .else
        	invoke crt_printf, offset g_szBpFmt, offset g_szCustomBp, [esi].ExceptionAddress
            ;还原
            invoke RestoreBp, [esi].ExceptionAddress
            mov eax, [esi].ExceptionAddress
        .endif
        invoke InputCommand
    .elseif [esi].ExceptionCode == EXCEPTION_SINGLE_STEP
    	invoke FindBp, [esi].ExceptionAddress
    	.if eax != 0
    		invoke SetBp, [esi].ExceptionAddress, 1
    	.endif
    	
    	.if g_bIsHardSingleStep == TRUE
            mov g_bIsHardSingleStep, FALSE
            ;重设硬件断点
            invoke OpenThread,THREAD_ALL_ACCESS,FALSE, g_de.dwThreadId
    		mov @hThread, eax
     		invoke GetContext, addr @ctx, @hThread
            or @ctx.iDr7, 1
    		invoke SetContext, addr @ctx, @hThread
    		invoke CloseHandle, @hThread
        .endif   
        
         .if g_bIsBmSingleStep 
            mov g_bIsBmSingleStep, FALSE
            
            ;重设内存断点
            invoke SetBmCommand
        .endif
        
        .if g_bIsSingleTStep == TRUE
            mov g_bIsSingleTStep, FALSE
            mov @bIsInputCmd, TRUE
    	.endif
    	
    	
    	;判断硬件执行断点是否触发
    	invoke OpenThread,THREAD_ALL_ACCESS, FALSE, g_de.dwThreadId
    	mov @hThread, eax
    		
        invoke GetContext, addr @ctx, @hThread
        .if g_bIsBhSingleStep == TRUE
        	.if @ctx.iDr6 & 1
            	invoke crt_printf, offset g_szBhTip, @ctx.regEip
            
            	and @ctx.iDr7, 0fffffffeh
            	or @ctx.regFlag, 100h
            	
            	invoke SetContext, addr @ctx, @hThread
            
            	mov g_bIsHardSingleStep, TRUE
	       
            	mov @bIsInputCmd, TRUE
         	.endif
        .endif

         
        .if g_bIsBaESingleStep == TRUE || g_bIsBaWSingleStep == TRUE
        	.if @ctx.iDr6 & 1
        		.if g_bIsBaESingleStep == TRUE
            		invoke crt_printf, offset g_szBaETip, @ctx.iDr0
            	.else
            		invoke crt_printf, offset g_szBaWTip, @ctx.iDr0
            	.endif 
            	mov @bIsInputCmd, TRUE
        	.endif
        .endif
        invoke CloseHandle, @hThread 
        .if @bIsInputCmd == TRUE
            invoke InputCommand
        .endif

    .elseif [esi].ExceptionCode == EXCEPTION_ACCESS_VIOLATION
        invoke VirtualProtectEx,g_hProc, g_dwMmbpAddr, g_dwMmbpLen, g_dwMmbpOld, addr @dwOldProc
        
        invoke SetTF
        mov g_bIsBmSingleStep, TRUE
        
        mov eax, [esi].ExceptionInformation[4]
        mov ecx, g_dwMmbpAddr
        add ecx, g_dwMmbpLen
        .if eax >= g_dwMmbpAddr && eax < ecx
            .if [esi].ExceptionInformation[0] == ACCESS_WRITE_EXCEPT
                invoke crt_printf, offset g_szBmTip, eax
                
                invoke InputCommand
            .endif
        .endif
    .endif
    
    mov eax, @dwStatus
    ret

OnExcept endp




main proc
    LOCAL @si:STARTUPINFO
    LOCAL @pi:PROCESS_INFORMATION
    LOCAL @dwStatus:DWORD
    
    invoke RtlZeroMemory, addr @si, size @si
    invoke RtlZeroMemory, addr @pi, size @pi
    
    ;建立调试会话
    invoke CreateProcess, NULL, offset g_szExe, NULL, NULL, FALSE, \
            DEBUG_ONLY_THIS_PROCESS or CREATE_NEW_CONSOLE, NULL, NULL, addr @si, addr @pi
            
    mov eax, @pi.hProcess
    mov g_hProc, eax
    
    invoke crt_printf, offset g_szSystemBegin
    ;循环接受调试事件
    .while TRUE
        mov @dwStatus, DBG_CONTINUE
        invoke RtlZeroMemory,offset g_de, size g_de
        invoke WaitForDebugEvent, offset g_de, INFINITE
        
        ;判断，处理
      	.if g_de.dwDebugEventCode == EXCEPTION_DEBUG_EVENT
            invoke OnExcept
            mov @dwStatus, eax
        .elseif g_de.dwDebugEventCode == OUTPUT_DEBUG_STRING_EVENT
            invoke OutputDebugHandler
        .endif
        ;提交处理结果
        invoke ContinueDebugEvent, g_de.dwProcessId, g_de.dwThreadId, @dwStatus
    .endw
    
    
    ret

main endp


ENTRY:

    invoke main
    invoke ExitProcess, 0
end ENTRY