.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	
.code 

SetBmCommand proc hProc:HANDLE, mmAddr:DWORD, mmLen:DWORD
    
    ;�޸��ڴ�����Ϊ���ɷ���
    invoke VirtualProtectEx, g_hProc, g_dwMmbpAddr, g_dwMmbpLen, PAGE_NOACCESS, offset g_dwMmbpOld
    
    ret

SetBmCommand endp

end