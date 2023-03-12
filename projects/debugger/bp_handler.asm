.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szBpListHeader               	db "序号      断点地址",0dh, 0ah, 0
   	g_szNoneBpList           		db "没有断点记录",0dh, 0ah, 0
   	g_szBpListFmt                  	db "%+4u      %+08X",0dh, 0ah, 0
	g_bpList bpStruct 100 dup(<?>) ;断点数组
	g_bpsLen dd 0 ;断点数组长度
.code 

ClearBpList proc uses ecx ebx edx hProc:HANDLE, dwAddr:DWORD
	mov ebx, offset g_bpList
	assume ebx:ptr bpStruct
	
	.if dwAddr == 0 || dwAddr == -1
		xor ecx, ecx
		.while ecx < g_bpsLen
			push ecx
			lea ecx, [ebx].m_oldCode
    		;还原指令
    		invoke WriteMemory, hProc, [ebx].m_dwAddr, ecx, 1
			pop ecx
			add ebx, size bpStruct
			inc ecx
		.endw
	
		mov g_bpsLen, 0
		invoke RtlZeroMemory,offset g_bpList, 500

	.elseif
		xor ecx, ecx
		.while ecx < g_bpsLen
			mov edx, dwAddr
			.if [ebx].m_dwAddr == edx
				push ecx
				lea ecx, [ebx].m_oldCode
    			;还原指令
    			invoke WriteMemory, hProc, [ebx].m_dwAddr, ecx, 1
				pop ecx
				invoke RtlZeroMemory, ebx, 5
				dec g_bpsLen
				jmp Arrange
			.endif
			add ebx, size bpStruct
			inc ecx
		.endw
Arrange:		
		.while ecx < g_bpsLen
			mov edx, ebx
			add edx, 5
			push ecx
			invoke RtlMoveMemory, ebx, edx, 5
			pop ecx
			inc ecx
		.endw
    .endif

	ret

ClearBpList endp

ShowBpList proc uses ecx ebx edx
	.if g_bpsLen == 0
		invoke crt_printf, offset g_szNoneBpList
		ret
	.endif
	mov ebx, offset g_bpList
	assume ebx:ptr bpStruct
	invoke crt_printf, offset g_szBpListHeader
	xor ecx, ecx
	.while ecx < g_bpsLen
		push ecx
		invoke crt_printf, offset g_szBpListFmt, ecx, [ebx].m_dwAddr
		pop ecx
		add ebx, size bpStruct
		inc ecx
	.endw
	ret

ShowBpList endp

FindBp proc uses ecx ebx dwAddr:DWORD
	mov eax, offset g_bpList
	assume eax:ptr bpStruct
	xor ecx, ecx
	.while ecx < g_bpsLen
		mov ebx, [eax].m_dwAddr
		.if dwAddr == ebx
			ret
		.endif
		add eax, size bpStruct
		inc ecx
	.endw
	mov eax, 0
	ret
FindBp endp

SetBp proc uses ecx ebx edx hProc:HANDLE, dwAddr:DWORD, dwFlag:DWORD
    LOCAL @dwBytesWriteReaded:DWORD
    LOCAL @btCC:BYTE
    
    .if dwFlag == 0
    	invoke FindBp, dwAddr
    	.if eax != 0
    		ret
    	.endif
    
    	;保存原指令
    	mov ecx, offset g_bpList
    	mov eax, g_bpsLen
    	mov ebx, size bpStruct
    	mul ebx
    	add ecx, eax
		assume ecx:ptr bpStruct
	
		lea edx, [ecx].m_oldCode
		push ecx
    	invoke ReadProcessMemory, hProc, dwAddr, edx, 1, addr @dwBytesWriteReaded
    	pop ecx
    	mov edx, dwAddr
    	mov [ecx].m_dwAddr, edx
    	inc g_bpsLen
    .endif
    	;写入CC
    	mov @btCC, 0cch
    	invoke WriteMemory, hProc, dwAddr, addr @btCC, size @btCC
    
    ret

SetBp endp

RestoreBp proc uses ebx ecx hProc:HANDLE, dwAddr:DWORD, pDe:DWORD
    invoke FindBp, dwAddr
    .if eax == 0
    	ret
    .endif
    
    mov ebx, eax
    assume ebx:ptr bpStruct
    
    lea ecx, [ebx].m_oldCode
    ;还原指令
    invoke WriteMemory, hProc, dwAddr, ecx, 1
    invoke SetTF, pDe 
    invoke DecEip, pDe
    ret

RestoreBp endp
end