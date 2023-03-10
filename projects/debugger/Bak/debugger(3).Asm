.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include disasm.inc
include utils.inc
include output_debug_handler.inc
include bp_handler.inc
include t_p_handler.inc
include ba_handler.inc

.data
	g_hProc  HANDLE 0
    g_de        DEBUG_EVENT <0>
    g_szCommand db 256 dup(0)
    g_bIsSysBp  db TRUE
	g_curExcptAddr dd 0
	g_dwPTFAddr dd 0
	
	g_dwTraceEnd dd 0
	
	g_bIsSingleTStep dd FALSE
	g_bIsSinglePStep dd FALSE
	g_bIsBhSingleStep dd FALSE  ;硬件执行断点的单步标志
	g_bIsBaESingleStep dd FALSE	; 硬件访问断点标志
	g_bIsBaWSingleStep dd FALSE	; 硬件写入断点标志
	g_bIsHardSingleStep dd FALSE; 硬件断点标志
	g_bIsBmSingleStep dd FALSE  ;内存断点的单步标志 
	
	g_dwMmbpAddr dd 0 ;内存断点地址
    g_dwMmbpLen  dd 0 ;内存断点长度 
    g_dwMmbpOld  dd 0 ;原来的内存保护属性
.code 


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
    invoke GetCurrEip,offset g_de
    mov @currDwEip, eax
    
    ;判断是否开启了trace
    mov eax, g_dwTraceEnd
    .if eax != 0
    	push eax
    	invoke PrintAsm, g_hProc, addr @curBufAsm, @currDwEip, addr @dwCodeLen, 1
    	pop eax
        ;判断trace是否结束
        .if eax != @currDwEip
            ;trace没有结束
            invoke SetTCommand, offset g_de, offset g_bIsSingleTStep
            ret
        .else
            ;trace结束
            mov g_dwTraceEnd, 0
        .endif
    .else
    	invoke PrintAsm, g_hProc, addr @curBufAsm, @currDwEip, addr @dwCodeLen, 5
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
            
            invoke SetBp, g_hProc,eax, 0
        .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'l'
        	invoke ShowBpList
        .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'c'
        	invoke crt_strtoul, offset g_szCommand + 3, NULL, 16
        	invoke ClearBpList, g_hProc,eax
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
            invoke SetTCommand,offset g_de, offset g_bIsSingleTStep
            
            ret 
        .elseif g_szCommand[0] == 't'
            invoke SetTCommand, offset g_de, offset g_bIsSingleTStep
            ret
        .elseif g_szCommand[0] == 'p'
        	invoke crt_strstr, addr @curBufAsm, offset g_szCall
        	 .if eax == NULL
        	 	invoke SetTCommand, offset g_de, offset g_bIsSingleTStep
            .else
            	mov g_bIsSinglePStep, TRUE
                mov eax, @dwCodeLen
            	add @currDwEip, eax
            	mov eax, @currDwEip
            	mov g_dwPTFAddr, eax
            	invoke SetPTF, g_hProc, @currDwEip
            .endif

            ret
        .elseif g_szCommand[0] == 'u'
        	invoke crt_strtoul, offset g_szCommand + 3, NULL, 16
            .if eax != 0 && eax != -1
             	mov @currDwEip, eax
            .endif
            invoke PrintAsm, g_hProc,addr @curBufAsm, @currDwEip, addr @dwCodeLen,5
  
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
        	invoke RestorePTF, g_hProc, g_dwPTFAddr, offset g_de, offset g_bIsSinglePStep
        	mov eax, g_dwPTFAddr
        	mov g_curExcptAddr, eax
        .else
        	invoke crt_printf, offset g_szBpFmt, offset g_szCustomBp, [esi].ExceptionAddress
            ;还原
            invoke RestoreBp, g_hProc,[esi].ExceptionAddress, offset g_de
            mov eax, [esi].ExceptionAddress
        .endif
        invoke InputCommand
    .elseif [esi].ExceptionCode == EXCEPTION_SINGLE_STEP
    	invoke FindBp, [esi].ExceptionAddress
    	.if eax != 0
    		invoke SetBp, g_hProc,[esi].ExceptionAddress, 1
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
        
        invoke SetTF, offset g_de
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
            invoke OutputDebugHandler, g_hProc, offset g_de
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