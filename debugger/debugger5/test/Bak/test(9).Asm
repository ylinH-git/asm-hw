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
includelib capstoneplus.lib


.data
    g_p dd 0
    g_dwIdx dd 0

    g_szFmt db "idx:%08X addr:%08X", 0dh, 0ah, 0
.code


ENTRY:
    invoke VirtualAlloc, NULL, 10000h, MEM_COMMIT,PAGE_EXECUTE_READWRITE
    mov g_p, eax
    
    mov eax, g_p
    add eax, 2000h
    mov [eax], eax
    
    mov g_dwIdx, 0
    .while g_dwIdx < 4000
        mov eax, g_p
        mov ebx, g_dwIdx
        
        
        
        mov dword ptr [eax+ebx*4], ecx
        
        add g_dwIdx, 1
    .endw

    invoke ExitProcess,0
end ENTRY