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
includelib disasm.lib

DisasmLine proto code:LPBYTE , nCodeSize:DWORD , nAddr:DWORD , szAsm:LPSTR