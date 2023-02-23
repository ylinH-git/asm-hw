
// petool.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#define Name(X) #X
#define InsertMember(X, LEN, ROW, LIST, OFFSET, NEEDNAME) { \
	CString fullName = Name(X); \
	CString memberName = fullName.Mid(LEN); \
	LIST.InsertItem(ROW, NEEDNAME ? memberName : ""); \
	CString sOffset; \
	sOffset.Format("0x%08X", OFFSET); \
	LIST.SetItemText(ROW, 1, sOffset); \
	CString sValue; \
	if(sizeof(X) > sizeof(short)) \
	{ sValue.Format("%08X", X); LIST.SetItemText(ROW, 2, "DWORD"); OFFSET+=4;}\
	else if(sizeof(X) > sizeof(char)) {sValue.Format("%04X", X); LIST.SetItemText(ROW, 2, "WORD"); OFFSET+=2;} \
	else {sValue.Format("%02X", X); LIST.SetItemText(ROW, 2, "BYTE"); OFFSET+=1;} \
	LIST.SetItemText(ROW, 3, sValue); \
}

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
	IMAGE_NT_HEADERS m_ntHeader = {};
	IMAGE_FILE_HEADER m_fileHeader = {};
	IMAGE_OPTIONAL_HEADER32 m_optional32Header = {};
	IMAGE_OPTIONAL_HEADER64 m_optional64Header = {};
	IMAGE_DOS_HEADER m_dosHeaderBuf = {};
	bool isx86 = true;

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CpetoolApp theApp;
