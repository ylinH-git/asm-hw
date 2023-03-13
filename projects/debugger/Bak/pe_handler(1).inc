GetPeData proto GetPeData proc pFilePath:DWORD
GetDosStructAddr proto
GetNtStructAddr proto
GetFileHStructAddr proto
GetOptionalStructAddr proto
GetDataDirStructAddr proto
GetSectionHeadersAddr proto
GetImportDirectoryAddr proto
GetDataDirStructLen proto
GetSectionHeadersLen proto
GetImportDirectoryLen proto
GetImportDllNameFAsAddr proto
GetFileAddr proto
GetRVAtoFA proto dwRva:DWORD
GetRVAtoVA proto dwRva:DWORD
GetFAtoRVA proto dwFa:DWORD
GetFAtoVA proto dwFa:DWORD
GetVAtoRVA proto dwVa:DWORD
GetVAtoFA proto dwVa:DWORD
FindFuncName proto hProc:DWORD, dwFuncAddr:DWORD
ClearPeData proto