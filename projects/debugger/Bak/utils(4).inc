GetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

SetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

WriteMemory proto hProc:HANDLE, destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD

GetCurrEip proto pDe:DWORD

PrintAsm proto hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD, asmNum: DWORD

DecEip proto pDe:DWORD

SetTF proto pDe:DWORD