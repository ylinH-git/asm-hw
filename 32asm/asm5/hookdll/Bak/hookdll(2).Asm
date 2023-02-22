.586
.model flat,stdcall
option casemap:none

   	include windows.inc
   	include user32.inc
   	include kernel32.inc
   	include shell32.inc
   	include msvcrt.inc
   
   	includelib user32.lib
   	includelib kernel32.lib
   	includelib shell32.lib
   	includelib msvcrt.lib

	InstallHook proto
	UninstallHook proto
	
.data 
	g_kernelbase db "kernelbase", 0
	g_CreateFileW db "CreateFileW", 0
	g_szFilePath db 512 dup(0)
	g_szRN db 0dh, 0ah, 0
	g_pDstAddr dd 0
	g_szLogFilePath db "D:\51asm\homework\32asm\asm5\hookdll\log.txt", 0
	g_dwBytesWirted dd 0
	g_overlapped OVERLAPPED <?>
.code

HOOK:
	; 保存环境
	; 所有通用寄存器压栈，8个dword
	pushad
	; 标志寄存器入栈 1个dword
	pushfd
	
	
	; 回复原来的代码
	invoke UninstallHook
	
	invoke RtlZeroMemory,offset g_overlapped, size g_overlapped
	
	mov ebx , [esp + 20h]
	invoke crt_wcslen, ebx
	mov ecx, eax
	invoke WideCharToMultiByte,65001, NULL, ebx, ecx, offset g_szFilePath, size g_szFilePath, NULL, NULL
	invoke CreateFile,offset g_szLogFilePath, GENERIC_READ or GENERIC_WRITE, FILE_SHARE_READ or FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL or FILE_FLAG_OVERLAPPED, NULL
	
	mov ebx, eax
	invoke crt_strlen, offset g_szFilePath
	mov ecx, eax
	mov g_overlapped.loffset, 0FFFFFFFFh
	mov g_overlapped.OffsetHigh, 0FFFFFFFFh
	invoke WriteFile, ebx, offset g_szFilePath , ecx, offset g_dwBytesWirted, offset g_overlapped
	mov g_overlapped.loffset, 0FFFFFFFFh
	mov g_overlapped.OffsetHigh, 0FFFFFFFFh
	invoke WriteFile, ebx, offset g_szRN, 3, offset g_dwBytesWirted, offset g_overlapped
	invoke CloseHandle, ebx
	
	; 重新安装installhook
	invoke InstallHook
	popfd
	popad
	
	; 执行被破坏的指令
	push ebp
	mov ebp, esp
	jmp g_pDstAddr


InstallHook proc

	LOCAL @pfnCreateFileW:DWORD
	LOCAL @dwOldProc
	
	invoke GetModuleHandle, offset g_kernelbase
	invoke GetProcAddress, eax, offset g_CreateFileW
	mov @pfnCreateFileW, eax
	
	invoke VirtualProtect, @pfnCreateFileW, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
	
	mov eax, @pfnCreateFileW
	; 操作码
	mov byte ptr[eax], 0e9h
	
	; 偏移
	mov ecx, offset HOOK
	sub ecx, @pfnCreateFileW
	sub ecx, 5
	mov dword ptr [eax + 1], ecx
	
	; 保存回调地址
	add eax,5
	mov g_pDstAddr, eax
	
	invoke VirtualProtect, @pfnCreateFileW, 1, @dwOldProc, addr @dwOldProc
	ret

InstallHook endp


UninstallHook proc
	LOCAL @pfnCreateFileW:DWORD
	LOCAL @dwOldProc
	
	invoke GetModuleHandle, offset g_kernelbase
	invoke GetProcAddress, eax, offset g_CreateFileW
	mov @pfnCreateFileW, eax
	
	invoke VirtualProtect, @pfnCreateFileW, 1, PAGE_EXECUTE_READWRITE, addr @dwOldProc
	
	mov eax, @pfnCreateFileW
	; 操作码
	mov byte ptr[eax], 8Bh
	mov byte ptr[eax + 1], 0FFh
	mov byte ptr[eax + 2], 55h
	mov byte ptr[eax + 3], 8Bh
	mov byte ptr[eax + 4], 0ECh
	
	
	invoke VirtualProtect, @pfnCreateFileW, 1, @dwOldProc, addr @dwOldProc
	ret

UninstallHook endp



DllMain proc   hinstDLL:HINSTANCE, fdwReason:DWORD, lpvReserved:LPVOID 

    .if fdwReason == DLL_PROCESS_ATTACH
        invoke InstallHook
    .endif

    mov eax, TRUE
    ret
DllMain endp


end DllMain