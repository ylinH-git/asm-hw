
// petool.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include <iostream>
#include <string>
#include <sstream>
#define Name(X) #X
#define InsertMember(X, LEN, ROW, LIST, OFFSET, NEEDNAME) { \
	CString fullName = Name(X); \
	CString memberName = fullName.Mid(LEN); \
	LIST.InsertItem(ROW, NEEDNAME ? memberName : ""); \
	CString sOffset; \
	sOffset.Format("%08X", OFFSET); \
	LIST.SetItemText(ROW, 1, sOffset); \
	CString sValue; \
	if(sizeof(X) == sizeof(ULONGLONG)) \
	{ sValue.Format("%016X", X); LIST.SetItemText(ROW, 2, "ULONGLONG"); OFFSET+=8;}\
	if(sizeof(X) > sizeof(short)) \
	{ sValue.Format("%08X", X); LIST.SetItemText(ROW, 2, "DWORD"); OFFSET+=4;}\
	else if(sizeof(X) > sizeof(char)) {sValue.Format("%04X", X); LIST.SetItemText(ROW, 2, "WORD"); OFFSET+=2;} \
	else {sValue.Format("%02X", X); LIST.SetItemText(ROW, 2, "BYTE"); OFFSET+=1;} \
	LIST.SetItemText(ROW, 3, sValue); \
}

struct sExportFunc {
	DWORD m_ordinal;
	DWORD m_funcRva;
	WORD m_nameOrdinal;
	DWORD m_nameRva;
	ULONG m_funcRvaOffset;
	ULONG m_nameOrdinalOffset;
	ULONG m_nameOffset;
	ULONG m_nameFa;
	char m_name[255];
};

// CpetoolApp:
// 有关此类的实现，请参阅 petool.cpp
//

class CpetoolApp : public CWinApp
{
public:
	CpetoolApp();

// 重写
public:
	virtual BOOL InitInstance();
	FILE* m_pFile = nullptr;
	ULONG m_ntOffset = 0;
	ULONG m_optionalOffset = 0;
	ULONG m_dataDirectoryOffset = 0;
	ULONG m_sectionHeaderOffset = 0;
	ULONG m_importDescriptorOffset = 0;
	ULONG m_importDescriptorFA = 0;
	ULONG m_exportDescriptorOffset = 0;
	ULONG m_exportDescriptorFA = 0;
	ULONG m_exportFuncsFA = 0;
	ULONG m_exportNamesFA = 0;
	ULONG m_exportNameOrdinalsFA = 0;
	ULONG m_relocationDirectoryFA = 0;
	ULONG m_relocationDirectoryOffset = 0;
	ULONG m_tlsDirectoryFA = 0;
	ULONG m_tlsValFA = 0;
	int m_importDescriptorLen = 0;
	int m_sectionHeaderLen = 0;
	int m_dataDirectoryLen = 0;
	int m_tlsValListNum = 0;
	IMAGE_NT_HEADERS m_ntHeader = {};
	IMAGE_FILE_HEADER m_fileHeader = {};
	IMAGE_OPTIONAL_HEADER32 m_optional32Header = {};
	IMAGE_OPTIONAL_HEADER64 m_optional64Header = {};
	IMAGE_DOS_HEADER m_dosHeaderBuf = {};
	IMAGE_EXPORT_DIRECTORY m_exportDirectory = {};
	IMAGE_TLS_DIRECTORY m_tlsDirectory = {};
	IMAGE_DATA_DIRECTORY* m_dataDirectoris = nullptr;
	IMAGE_SECTION_HEADER* m_sectionHeaders = nullptr;
	IMAGE_IMPORT_DESCRIPTOR* m_importDescriptors = nullptr;
	IMAGE_BASE_RELOCATION* m_baseRelocation = nullptr;
	sExportFunc* m_exportFuncList = nullptr;
	DWORD* m_tlsValList = nullptr;
	ULONG* m_importDllNameFAs = nullptr;
	bool isx86 = true;

// 实现
	ULONG GetRVAtoFA(ULONG rva);
	ULONG GetRVAtoVA(ULONG rva);
	ULONG GetFAtoRVA(ULONG fa);
	ULONG GetFAtoVA(ULONG fa);
	ULONG GetVAtoFA(ULONG va);
	ULONG GetVAtoRVA(ULONG va);
	ULONGLONG GetHex(std::string str);

	DECLARE_MESSAGE_MAP()
};

extern CpetoolApp theApp;
