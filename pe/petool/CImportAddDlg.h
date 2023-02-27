#pragma once
#include "afxdialogex.h"


// CImportAddDlg 对话框

class CImportAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportAddDlg)

public:
	CImportAddDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImportAddDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_IMPORT_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_filePath;
	afx_msg void OnBnClickedDll();
	CString m_funcName;
	afx_msg void OnBnClickedFunc();
	CListBox m_dllList;
	CListBox m_funcLIst;
	CString m_dllPaths[100] = {};
	CString m_dllNames[100] = {};
	CString m_funcNames[100][500] = {};
	afx_msg void OnBnClickedInsert();
	afx_msg void OnDblclkListDll();
};
