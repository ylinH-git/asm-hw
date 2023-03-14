SetPTF proto hProc:HANDLE, dwAddr:DWORD

RestorePTF proto hProc:HANDLE, dwPTFAddr:DWORD, pDe:DWORD, pBIsPStep:DWORD

SetTCommand proto pDe:DWORD, pBIsTStep:DWORD

HandlerPCommand proto hProc:DWORD, pDe:DWORD, pCurBufAsm:DWORD, pBIsSingleTStep:DWORD, dwCodeLen:DWORD, pCurrDwEip:DWORD, pDwPTFAddr:DWORD