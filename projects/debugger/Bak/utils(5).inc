GetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

SetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

WriteMemory proto hProc:HANDLE, destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD

GetCurrEip proc uses esi pDe:DWORD