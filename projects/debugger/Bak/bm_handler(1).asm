.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc


.code 

SetBmCommand proc hProc:HANDLE, mmBpAddr:DWORD, mmBpLen:DWORD, pMmbpOld:DWORD
    
    ;修改内存属性为不可访问
    invoke VirtualProtectEx, g_hProc, mmBpAddr, mmBpLen, PAGE_NOACCESS, pMmbpOld
    
    ret

SetBmCommand endp

end