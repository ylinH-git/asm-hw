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


    public g_hProc  HANDLE 0
