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


ACCESS_READ_EXCEPT equ 0 
ACCESS_WRITE_EXCEPT equ 1

bpStruct struc
	m_oldCode db 0
	m_dwAddr  dd 0
bpStruct ends


.const
	g_szExe 						db "test.exe", 0
	g_szSystemBegin 				db "start debugging...", 0dh, 0ah, 0
    g_szOUTPUT_DEBUG_STRING_EVENT  	db "OUTPUT_DEBUG_STRING_EVENT",0
    g_szCommandErrTip              	db "�������", 0dh, 0ah, 0
    g_szBpListHeader               	db "���      �ϵ��ַ",0dh, 0ah, 0
   	g_szNoneBpList           		db "û�жϵ��¼",0dh, 0ah, 0
   	g_szSystemBp					db "����ϵͳ�ϵ�:0x", 0
   	g_szCustomBp					db "����һ��ϵ�:0x", 0
   	g_szBhTip                       db "����Ӳ��ִ�жϵ� %08X", 0dh, 0ah, 0
   	g_szBaETip                      db "����Ӳ�����ʶϵ� %08X", 0dh, 0ah, 0
   	g_szBaWTip                      db "����Ӳ��д��ϵ� %08X", 0dh, 0ah, 0
   	g_szBmTip                      	db "�����ڴ�д��ϵ� %08X", 0dh, 0ah, 0
    g_szFmt                        	db "%s",0dh, 0ah, 0
    g_szBpFmt                       db "%s%+08X",0dh, 0ah, 0
    g_szBpListFmt                  	db "%+4u      %+08X",0dh, 0ah, 0
    g_szAsmFmt                      db "0x%08X %s",0dh, 0ah, 0
    g_szCurAsmFmt                   db "0x%08X %s <==== ",0dh, 0ah, 0
    g_szCall    					db "call", 0
    
  