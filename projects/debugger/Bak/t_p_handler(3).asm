.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc
include bp_handler.inc

externdef g_bIsSingleTStep:dword
externdef g_bIsSinglePStep:dword

.data
	g_pPTFOldCode dd 0
.code 

SetPTF proc uses ecx ebx edx hProc:HANDLE, dwAddr:DWORD
    LOCAL @dwBytesWriteReaded:DWORD
    LOCAL @btCC:BYTE
    invoke ReadProcessMemory, hProc, dwAddr, offset g_pPTFOldCode, 1, addr @dwBytesWriteReaded
    
    ;写入CC
    mov @btCC, 0cch
    invoke WriteMemory, hProc, dwAddr, addr @btCC, size @btCC
    
    ret

SetPTF endp

RestorePTF proc uses ebx ecx hProc:HANDLE, dwPTFAddr:DWORD, pDe:DWORD
	invoke FindBp, dwPTFAddr
    .if eax != 0
    	mov ebx, eax
    	assume ebx:ptr bpStruct
    
    	lea ecx, [ebx].m_oldCode
    	;还原指令
    	invoke WriteMemory, hProc, dwPTFAddr, ecx, 1
    	invoke SetTF, pDe
    .else
    	;还原指令
    	invoke WriteMemory, hProc, dwPTFAddr, offset g_pPTFOldCode , 1
    .endif
    invoke DecEip, pDe
    mov g_bIsSinglePStep, FALSE
    ret

RestorePTF endp

SetTCommand proc pDe:DWORD, pBIsTStep:DWORD
	mov eax, pBIsTStep
  	mov dword ptr [eax], TRUE
  	invoke SetTF, pDe
   	ret
SetTCommand endp
end