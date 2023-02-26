GetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

SetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

WriteMemory proto hProc:HANDLE, destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD

GetCurrEip proc uses esi pDe:DWORD

PrintAsm proc uses ecx hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD, asmNum: DWORD

DecEip proc uses esi pDe:DWORD

SetTF proc uses esi, pDe:DWORD