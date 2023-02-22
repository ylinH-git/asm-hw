.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive


include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc
include msvcrt.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib
includelib msvcrt.lib

bpStruct struc
	m_oldCode db 0
	m_dwAddr  dd 0
bpStruct ends

.const
	g_szExe 						db "winmine.exe", 0
	g_szSystemBegin 				db "start debugging...", 0dh, 0ah, 0
    g_szOUTPUT_DEBUG_STRING_EVENT  	db "OUTPUT_DEBUG_STRING_EVENT",0
    g_szCommandErrTip              	db "命令出错", 0dh, 0ah, 0
    g_szBpListHeader               	db "序号      断点地址",0dh, 0ah, 0
   	g_szNoneBpList           		db "没有断点记录",0dh, 0ah, 0
   	g_szSystemBp					db "进入系统断点:0x", 0
   	g_szCustomBp					db "进入一般断点:0x", 0
    g_szFmt                        	db "%s",0dh, 0ah, 0
    g_szBpFmt                       db "%s%+08x",0dh, 0ah, 0
    g_szBpListFmt                  	db "%+4u      %+08x",0dh, 0ah, 0

.data
    g_de        DEBUG_EVENT <0>
    g_hProc     HANDLE 0
    g_szCommand db 256 dup(0)
    g_bIsSysBp  db TRUE
    g_btOldCode db 0 ;int 3 原指令
    g_dwBpAddr  dd 0
	g_bpList bpStruct 100 dup(<?>) ;断点数组
	g_bpsLen dd 0 ;断点数组长度
	g_curExcptAddr dd 0
.code 

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

RestoreBp proc uses ebx ecx dwAddr:DWORD
    LOCAL @dwBytesWriteReaded:DWORD
    LOCAL @ctx:CONTEXT
    LOCAL @hThread:HANDLE
    
    invoke FindBp, dwAddr
    .if eax == 0
    	ret
    .endif
    
    mov ebx, eax
    assume ebx:ptr bpStruct
    
    lea ecx, [ebx].m_oldCode
    ;还原指令
    invoke WriteMemory, dwAddr, ecx, 1
    
    ;回退EIP，设TF
    ;TF :调试标志位。 当TF=1时，处理器每次只执行一条指令，即单步执行
    invoke OpenThread,THREAD_ALL_ACCESS, FALSE, g_de.dwThreadId
    mov @hThread, eax
    
    mov @ctx.ContextFlags, CONTEXT_CONTROL
    invoke GetThreadContext, @hThread, addr @ctx
    
    dec @ctx.regEip
    or @ctx.regFlag, 100h
    
    invoke SetThreadContext, @hThread, addr @ctx
    
    
    ret

RestoreBp endp


InputCommand proc uses ebx 
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
            
            ;设置断点
            mov g_dwBpAddr, eax
            invoke SetBp, eax, 0
        .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'l'
        	invoke ShowBpList
        .elseif g_szCommand[0] == 'b' && g_szCommand[1] == 'c'
        	invoke crt_strtoul, offset g_szCommand + 3, NULL, 16
        	invoke ClearBpList, eax
        .elseif g_szCommand[0] == 'g'
            ret
        .endif
    .endw

    ret

InputCommand endp


OnExcept proc
    LOCAL @dwStatus:DWORD

    lea esi, g_de.u.Exception.pExceptionRecord
    assume esi:ptr EXCEPTION_RECORD
    
    
    mov @dwStatus, DBG_CONTINUE
    
    .if [esi].ExceptionCode == EXCEPTION_BREAKPOINT
        ;判断是否是系统断点
        .if g_bIsSysBp
        	invoke crt_printf, offset g_szBpFmt, offset g_szSystemBp, [esi].ExceptionAddress
            mov g_bIsSysBp, FALSE
        .else
        	invoke crt_printf, offset g_szBpFmt, offset g_szCustomBp, [esi].ExceptionAddress
            ;还原
            invoke RestoreBp, [esi].ExceptionAddress
            mov eax, [esi].ExceptionAddress
            mov g_curExcptAddr, eax
        .endif
        invoke InputCommand
    .elseif [esi].ExceptionCode == EXCEPTION_SINGLE_STEP
    	invoke FindBp, g_curExcptAddr
    	.if eax != 0
    		invoke SetBp, g_curExcptAddr, 1
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
            DEBUG_ONLY_THIS_PROCESS, NULL, NULL, addr @si, addr @pi
            
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