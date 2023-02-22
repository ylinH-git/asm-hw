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


.const
	g_szExe db "outputDebug.exe", 0
    g_szOUTPUT_DEBUG_STRING_EVENT  db "OUTPUT_DEBUG_STRING_EVENT",0
    
    g_szFmt                        db "%s",0dh, 0ah, 0
    g_szLoadDllFmt                 db "%08X %s", 0dh, 0ah, 0
    
.data
    g_de        DEBUG_EVENT <0>
    g_hProc     HANDLE 0
    g_szCommand db 256 dup(0)


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
    
    mov ebx, [esi].nDebugStringiLength
    inc ebx
    invoke crt_malloc, ebx
    mov @dwBufAddr, eax
    invoke RtlZeroMemory,@dwBufAddr, ebx
    invoke ReadProcessMemory, g_hProc, [esi].lpDebugStringData,  @dwBufAddr, ebx, addr @dwBytesReaded
    .if !eax
         ret
    .endif
    .if [esi].fUnicode

        invoke WideCharToMultiByte, CP_ACP, 0, @dwBufAddr, -1, addr @szBuf, ebx, NULL, NULL
        invoke crt_wprintf, offset g_szFmt, @dwBufAddr
     .else
       	invoke crt_printf, offset g_szFmt, @dwBufAddr
    .endif
    
    ;输出


	invoke crt_free, @dwBufAddr
	ret

OutputDebugHandler endp

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
    
    
    ;循环接受调试事件
    .while TRUE
        mov @dwStatus, DBG_CONTINUE
        invoke RtlZeroMemory,offset g_de, size g_de
        invoke WaitForDebugEvent, offset g_de, INFINITE
        
        ;判断，处理
      	.if g_de.dwDebugEventCode == OUTPUT_DEBUG_STRING_EVENT
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