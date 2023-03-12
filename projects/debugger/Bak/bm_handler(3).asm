.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szBmListHeader               	db "序号    断点地址    长度    类型",0dh, 0ah, 0
   	g_szNoneBmList           		db "没有内存断点记录",0dh, 0ah, 0
   	g_szBmListFmt                  	db "%+4u    %+08X    %4d    %c",0dh, 0ah, 0
   	g_szBmPListHeader               db "分页地址    断点地址    断点标号    旧属性      新属性",0dh, 0ah, 0
   	g_szBmPListFmt                  db "%+08X    %+08X    %+8X    %+08X    %+08X",0dh, 0ah, 0
	g_bmList bmStruct 100 dup(<?>) ;内存断点数组
	g_bmsLen dd 0 ;内存断点数组长度
	
.code 


ClearBmList proc uses ecx ebx edx hProc:HANDLE, dwIdx:DWORD
	LOCAL @mmBpOld:DWORD
	mov ebx, offset g_bmList
	assume ebx:ptr bmStruct
	
	.if dwIdx == -1
		xor ecx, ecx
		.while ecx < g_bmsLen
			push ecx
    		;还原内存属性
    		invoke VirtualProtectEx, hProc, [ebx].m_dwAddr, [ebx].m_dwLen, [ebx].m_dwOldAttr, addr @mmBpOld
			pop ecx
			add ebx, size bmStruct
			inc ecx
		.endw
	
		mov g_bmsLen, 0
		invoke RtlZeroMemory,offset g_bmList, 2400

	.elseif
		mov ebx, dwIdx
		mov eax, size bmStruct
		mul ebx
		add ebx, offset g_bmList
		assume ebx:ptr bmStruct
		push ecx
    		;还原内存属性
    		invoke VirtualProtectEx, hProc, [ebx].m_dwAddr, [ebx].m_dwLen, [ebx].m_dwOldAttr, addr @mmBpOld
		pop ecx
		invoke RtlZeroMemory, ebx, size bmStruct
		dec g_bmsLen
		mov ecx, dwIdx
Arrange:		
		.while ecx < g_bmsLen
			mov edx, ebx
			add edx, size bmStruct
			push ecx
			invoke RtlMoveMemory, ebx, edx, size bmStruct
			pop ecx
			inc ecx
		.endw
    .endif

	ret

ClearBmList endp


ShowBmList proc uses ecx ebx edx
	.if g_bmsLen == 0
		invoke crt_printf, offset g_szNoneBmList
		ret
	.endif
	mov ebx, offset g_bmList
	assume ebx:ptr bmStruct
	invoke crt_printf, offset g_szBmListHeader
	xor ecx, ecx
	.while ecx < g_bmsLen
		push ecx
		.if [ebx].m_dwType == 0
			invoke crt_printf, offset g_szBmListFmt, ecx, [ebx].m_dwAddr, [ebx].m_dwLen, 'w'
		.elseif [ebx].m_dwType == 1
			invoke crt_printf, offset g_szBmListFmt, ecx, [ebx].m_dwAddr, [ebx].m_dwLen, 'r'
		.endif
		pop ecx
		add ebx, size bmStruct
		inc ecx
	.endw
	ret

ShowBmList endp


ShowBmPList proc uses ecx ebx edx
	.if g_bmsLen == 0
		invoke crt_printf, offset g_szNoneBmList
		ret
	.endif
	mov ebx, offset g_bmList
	assume ebx:ptr bmStruct
	invoke crt_printf, offset g_szBmPListHeader
	xor ecx, ecx
	.while ecx < g_bmsLen
		push ecx
		invoke crt_printf, offset g_szBmPListFmt, [ebx].m_dwPage,[ebx].m_dwAddr, ecx, [ebx].m_dwOldAttr, [ebx].m_dwNewAttr
		pop ecx
		add ebx, size bmStruct
		inc ecx
	.endw
	ret

ShowBmPList endp


FindBm proc uses ecx ebx edx dwAddr:DWORD
	mov eax, offset g_bmList
	assume eax:ptr bmStruct
	xor ecx, ecx
	.while ecx < g_bmsLen
		mov ebx, [eax].m_dwAddr
		mov edx, [eax].m_dwLen
		add edx, ebx
		.if dwAddr >= ebx && dwAddr < edx
			ret
		.endif
		add eax, size bmStruct
		inc ecx
	.endw
	mov eax, 0
	ret
FindBm endp

FindBmInPage proc uses ecx ebx edx dwAddr:DWORD
	mov eax, offset g_bmList
	assume eax:ptr bmStruct
	xor ecx, ecx
	.while ecx < g_bmsLen
		mov ebx, [eax].m_dwPage
		mov edx, [eax].m_dwPage
		add edx, 1000h
		.if dwAddr >= ebx && dwAddr < edx
			ret
		.endif
		add eax, size bmStruct
		inc ecx
	.endw
	mov eax, 0
	ret
FindBmInPage endp

ArrangeList proc uses ecx ebx edx esi edi
	LOCAL @tempStruct: bmStruct
	
	xor ecx, ecx
	mov ebx, g_bmsLen
	dec ebx
	.while ecx < ebx
		mov eax, size bmStruct
		mul ecx
		add eax, offset g_bmList
		mov esi, eax
		assume esi:ptr bmStruct
		mov edx, ecx
		inc edx
		.while edx < g_bmsLen
			push edx
			mov eax, size bmStruct
			mul edx
			add eax, offset g_bmList
			mov edi, eax
			assume edi:ptr bmStruct
			pop edx
			
			mov eax, [edi].m_dwPage
			.if eax < [esi].m_dwPage
				push ecx
				push edx
				invoke RtlMoveMemory, addr @tempStruct, esi, size bmStruct
				invoke RtlMoveMemory, esi, edi, size bmStruct
				invoke RtlMoveMemory, edi, addr @tempStruct, size bmStruct
				pop edx
				pop ecx
			.endif
			
			mov eax, [edi].m_dwPage
			.if eax == [esi].m_dwPage
				mov eax, [edi].m_dwAddr
				.if eax < [esi].m_dwAddr
					push ecx
					push edx
					invoke RtlMoveMemory, addr @tempStruct, esi, size bmStruct
					invoke RtlMoveMemory, esi, edi, size bmStruct
					invoke RtlMoveMemory, edi, addr @tempStruct, size bmStruct
					pop edx
					pop ecx
				.endif
			.endif
			
			inc edx
		.endw
		inc ecx
	.endw
	ret

ArrangeList endp

SetBmCommand proc uses esi ecx ebx edx hProc:HANDLE, mmBmAddr:DWORD, mmBmLen:DWORD, mmBmType:DWORD
	LOCAL @currPage: DWORD
	LOCAL @restBytesWrite: DWORD
	LOCAL @splitPageLen: DWORD
	LOCAL @oldPageAttr: DWORD
	
	mov @oldPageAttr, 0
	
	invoke FindBmInPage, mmBmAddr
    .if eax != 0
    	mov esi, eax
    	assume esi:ptr bmStruct
    	mov @oldPageAttr,[esi].m_dwOldAttr 
    .endif 
    
    mov esi, offset g_bmList
    assume esi:ptr bmStruct
   
    
    xor ecx, ecx
    .while ecx < g_bmsLen
    	mov ebx, [esi].m_dwAddr
    	mov edx, [esi].m_dwAddr
    	add edx, [esi].m_dwLen
    	.if mmBmAddr >= ebx && mmBmAddr <= edx
    		ret
    	.endif
    	
    	mov eax, mmBmAddr
    	add eax, mmBmLen
    	.if eax >= ebx && eax <= edx
    		ret
    	.endif
  	
		add esi, size bmStruct
		inc ecx
	.endw
    
    mov esi, offset g_bmList
    assume esi:ptr bmStruct
    mov eax, g_bmsLen
    mov ecx, size bmStruct
    mul ecx
    add esi, eax
    
    mov ebx, mmBmAddr
    shr ebx, 12
    shl ebx, 12
    mov @currPage, ebx
    
    xor edx, edx
    mov eax, mmBmAddr
    add eax, mmBmLen
    sub eax, ebx
    
    .if eax > 0
    	mov ebx, 1000h
    	div ebx
    	mov @splitPageLen, eax
    .endif
    
    mov eax, mmBmLen
    mov @restBytesWrite, eax
    .if @splitPageLen > 0
    	xor ecx, ecx
    	.while ecx <= @splitPageLen
    		mov eax, ecx
    		add eax, g_bmsLen
    		.break .if eax >= 100
    		mov eax, @currPage
    		mov [esi].m_dwPage, eax
    		.if ecx == 0
    			mov eax, mmBmAddr
    			mov [esi].m_dwAddr, eax
    			mov eax, @currPage
    			add eax, 1000h
    			sub eax, mmBmAddr
    			mov [esi].m_dwLen, eax
    			sub @restBytesWrite, eax
    		.elseif 
    			mov eax, @currPage
    			mov [esi].m_dwAddr, eax
    			.if @restBytesWrite > 1000h
    				mov [esi].m_dwLen, 1000h
    				sub @restBytesWrite, 1000h
    			.elseif
    				mov eax, @restBytesWrite
    				mov [esi].m_dwLen, eax
    				sub @restBytesWrite, eax
    			.endif
    		.endif
    		mov eax, mmBmType
    		mov [esi].m_dwType, eax
    		push ecx
    		;修改内存属性为不可访问
    		invoke VirtualProtectEx, hProc, [esi].m_dwAddr, [esi].m_dwLen, PAGE_NOACCESS, addr [esi].m_dwOldAttr
    		pop ecx
    		.if @oldPageAttr != 0
    			mov eax, @oldPageAttr
    			mov [esi].m_dwOldAttr, eax
    		.endif
    		mov [esi].m_dwNewAttr, PAGE_NOACCESS
    		add @currPage, 1000h
    		inc ecx
    		inc g_bmsLen
    		add esi, size bmStruct
    	.endw
    .elseif
    	mov eax, mmBmAddr
    	shr eax, 12
    	shl eax, 12
    	mov [esi].m_dwPage, eax
    	mov eax, mmBmAddr
    	mov [esi].m_dwAddr, eax
    	mov eax, mmBmLen
    	mov [esi].m_dwLen, eax
    	mov eax, mmBmType
    	mov [esi].m_dwType, eax
    	;修改内存属性为不可访问
    	invoke VirtualProtectEx, hProc, mmBmAddr, mmBmLen, PAGE_NOACCESS, addr [esi].m_dwOldAttr
    	mov [esi].m_dwNewAttr, PAGE_NOACCESS
    	inc g_bmsLen
    .endif
	
    invoke ArrangeList
    ret

SetBmCommand endp

ResetBmCommand proc uses esi hProc:HANDLE, dwAddr:DWORD
	mov esi, dwAddr
	assume esi:ptr bmStruct
	invoke VirtualProtectEx, hProc, [esi].m_dwAddr, [esi].m_dwLen, PAGE_NOACCESS, addr [esi].m_dwOldAttr
	mov [esi].m_dwNewAttr, PAGE_NOACCESS
	ret

ResetBmCommand endp

end