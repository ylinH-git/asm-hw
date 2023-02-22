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

    g_szBaseFmt db "base:%08X", 0dh, 0ah, 0
    g_szFmt db "base:%08X idx:%08X addr:%08X", 0dh, 0ah, 0
.code


ENTRY:
    invoke VirtualAlloc, NULL, 10000h, MEM_COMMIT,PAGE_EXECUTE_READWRITE
    mov g_p, eax
    
    invoke crt_printf, offset g_szBaseFmt, g_p
    
    mov eax, g_p
    add eax, 2000h
    mov [eax], eax
    
    mov g_dwIdx, 0
    mov esi, g_p
    .while g_dwIdx < 4000h
        mov ebx, g_dwIdx
        
        lea ecx, dword ptr [esi+ebx*4]
        invoke crt_printf, offset g_szFmt, g_p, ebx, ecx
        
        mov dword ptr [esi+ebx*4], ebx
        
        add g_dwIdx, 1
    .endw

    invoke ExitProcess,0
end ENTRY