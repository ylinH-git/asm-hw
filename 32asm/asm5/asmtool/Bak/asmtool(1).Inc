
include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib

DlgProc			PROTO	:HWND,:UINT,:WPARAM,:LPARAM

.const

IDD_DIALOG1			equ 101
BTN_INJECT			equ 1001
EDT_PID				equ 1002
EDT_ASM				equ 1004

;#########################################################################

.data?

hInstance			dd ?

;#########################################################################
