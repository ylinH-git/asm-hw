GetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

SetContext proto pCtx:ptr CONTEXT, hThread:HANDLE

WriteMemory proto hProc:HANDLE, destAddr:DWORD, sourceAddr:DWORD, memSize: DWORD

ReadMemory proto hProc:HANDLE, dwAddr:DWORD, dwSize:DWORD, pBuf:DWORD  

ReadMemoryPartlyFromProcess proto hProc:DWORD,dwAddr:DWORD, dwSize:DWORD, pBuf:DWORD 

IsMemoryNotNull proto pData:DWORD, dataSize:DWORD

GetCurrEip proto pDe:DWORD

GetAsm proto hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD

PrintAsm proto hProc:HANDLE, pCurBufAsm:DWORD, currDwEip:DWORD, pDwCodeLen:DWORD, asmNum: DWORD

DecEip proto pDe:DWORD

SetTF proto pDe:DWORD

SplitPath proto pFilePath:DWORD

GetFileName proto

GetFileExt proto