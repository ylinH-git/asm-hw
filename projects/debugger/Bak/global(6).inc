include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc
include msvcrt.inc
include psapi.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib
includelib msvcrt.lib
includelib psapi.lib


ACCESS_READ_EXCEPT equ 0 
ACCESS_WRITE_EXCEPT equ 1

bpStruct struc
	m_oldCode db 0
	m_dwAddr  dd 0
bpStruct ends


bmStruct struc
	m_dwPage db 0
	m_dwOldAttr  dd 0
	m_dwNewAttr  dd 0
bmStruct ends



.const
	g_szExe 						db "test.exe", 0
	g_szSystemBegin 				db "start debugging...", 0dh, 0ah, 0
    g_szOUTPUT_DEBUG_STRING_EVENT  	db "OUTPUT_DEBUG_STRING_EVENT",0
    g_szCommandErrTip              	db "命令出错", 0dh, 0ah, 0
   	g_szSystemBp					db "命中系统断点:0x", 0
   	g_szCustomBp					db "命中一般断点:0x", 0
   	g_szBhETip                       db "命中硬件执行断点 %08X", 0dh, 0ah, 0
   	g_szBhATip                      db "命中硬件访问断点 %08X", 0dh, 0ah, 0
   	g_szBhWTip                      db "命中硬件写入断点 %08X", 0dh, 0ah, 0
   	g_szBmTip                      	db "命中内存写入断点 %08X", 0dh, 0ah, 0
    g_szFmt                        	db "%s",0dh, 0ah, 0
    g_szBpFmt                       db "%s%+08X",0dh, 0ah, 0
    g_szAsmFmt                      db "0x%08X %s",0dh, 0ah, 0
    g_szCurAsmFmt                   db "0x%08X %s <==== ",0dh, 0ah, 0
    g_szCall    					db "call", 0
    g_szJmp   						db "jmp", 0
    g_szPtr   						db "ptr", 0
    g_szFuncAsmFmt                  db "0x%08X %s <%s>",0dh, 0ah, 0
    g_szCurFuncAsmFmt               db "0x%08X %s <%s> <==== ",0dh, 0ah, 0
    g_szRn 							db 0dh, 0ah, 0
  
