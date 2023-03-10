GetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

SetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

WriteMemory proto hProc:HANDLE, destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD

GetCurrEip proto uses esi pDe:DWORD

PrintAsm proto uses ecx hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD, asmNum: DWORD

DecEip proto uses esi pDe:DWORD

SetTF proto uses esi, pDe:DWORD