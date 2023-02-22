.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include kernel32.inc
include windows.inc
include user32.inc

includelib kernel32.lib
includelib user32.lib

MINIDUMP_EXCEPTION_INFORMATION struc  
  ThreadId DWORD 0             
  ExceptionPointers DWORD 0 
  ClientPointers  BOOL 0
MINIDUMP_EXCEPTION_INFORMATION ends



.data
	g_szFileName db "dump.dmp", 0
	g_miniDumpWriteDump db "MiniDumpWriteDump", 0
	g_dbghelp db "DbgHelp.dll", 0
	g_minidump_exception_info MINIDUMP_EXCEPTION_INFORMATION <?>
.code 

MyUnhandledExceptionFilter proc uses esi edi  ExceptionInfo: ptr EXCEPTION_POINTERS 
	LOCAL @hFile: HANDLE
	LOCAL @dwPid: DWORD
	LOCAL @hProc:HANDLE
	LOCAL @pfnMiniDumpWriteDump
    mov esi, ExceptionInfo
    assume esi:ptr EXCEPTION_POINTERS
    mov edi, [esi].ContextRecord
    assume edi:ptr CONTEXT
    mov esi, [esi].pExceptionRecord
    assume esi:ptr EXCEPTION_RECORD
    
    invoke CreateFile, offset g_szFileName,GENERIC_READ or GENERIC_WRITE,FILE_SHARE_WRITE or FILE_SHARE_READ, NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL
    mov @hFile, eax
    invoke GetCurrentProcessId
    mov @dwPid, eax
    invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, @dwPid
    mov @hProc, eax
    
    invoke LoadLibrary, offset g_dbghelp
	invoke GetProcAddress, eax, offset g_miniDumpWriteDump
	mov @pfnMiniDumpWriteDump, eax
	
	mov eax, ExceptionInfo
	mov g_minidump_exception_info.ExceptionPointers, eax
	
	push NULL
	push NULL
	push offset g_minidump_exception_info
	push @hFile
	push @dwPid
	push @hProc
	call @pfnMiniDumpWriteDump
    ret

MyUnhandledExceptionFilter endp

ENTRY:
    ;注册异常回调函数
    invoke SetUnhandledExceptionFilter, offset MyUnhandledExceptionFilter
    
	mov eax, 123
	mov [eax], eax

end ENTRY