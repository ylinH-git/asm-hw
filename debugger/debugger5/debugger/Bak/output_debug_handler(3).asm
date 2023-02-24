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
    ; ��ȥ������ϵͳ��ͨ�� OutputDebugStringW ���� Unicode �ַ��������� ���� ASCII �ַ���
    ; ��Ҫǿ�� OutputDebugStringW ���� Unicode �ַ�������������Ҫ���� WaitForDebugEventEx ������ѡ���������Ϊ�� 
    ; ����������ϵͳ��֪��������֧�� Unicode����ר��ѡ����� Unicode �ַ�����
    ; ������ʱֻ���߶��ֽڵķ�֧
    .if [esi].fUnicode
        invoke crt_wprintf, offset g_szFmt, @dwBufAddr
     .else
       	invoke crt_printf, offset g_szFmt, @dwBufAddr
    .endif

	invoke crt_free, @dwBufAddr
	ret

OutputDebugHandler endp
end