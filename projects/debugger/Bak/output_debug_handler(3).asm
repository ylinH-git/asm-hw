.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc

.code 
OutputDebugHandler proc uses esi ebx hProc:HANDLE, pDe: DWORD
	LOCAL @szBuf[MAX_PATH]:BYTE
    LOCAL @szBufW[MAX_PATH]:WORD
    LOCAL @dwBytesReaded:DWORD
    LOCAL @dwBufAddr: DWORD
    
    
    invoke crt_printf, offset g_szFmt, offset g_szOUTPUT_DEBUG_STRING_EVENT
    
    invoke RtlZeroMemory,addr @szBuf, MAX_PATH
    
    mov esi, pDe
    assume esi: ptr DEBUG_EVENT
    lea esi, pDe.u.DebugString
    assume esi:ptr OUTPUT_DEBUG_STRING_INFO
    
    xor ebx, ebx
    mov bx, [esi].nDebugStringiLength
    inc ebx
    invoke crt_malloc, ebx
    mov @dwBufAddr, eax
    invoke RtlZeroMemory,@dwBufAddr, ebx
    invoke ReadProcessMemory, hProc, [esi].lpDebugStringData,  @dwBufAddr, ebx, addr @dwBytesReaded
    .if !eax
         ret
    .endif
    ; 过去，操作系统不通过 OutputDebugStringW 返回 Unicode 字符串，而是 返回 ASCII 字符串
    ; 若要强制 OutputDebugStringW 返回 Unicode 字符串，调试器需要调用 WaitForDebugEventEx 函数以选择加入新行为。 
    ; 这样，操作系统就知道调试器支持 Unicode，并专门选择接收 Unicode 字符串。
    ; 所以暂时只会走多字节的分支
    .if [esi].fUnicode
        invoke crt_wprintf, offset g_szFmt, @dwBufAddr
     .else
       	invoke crt_printf, offset g_szFmt, @dwBufAddr
    .endif

	invoke crt_free, @dwBufAddr
	ret

OutputDebugHandler endp
end