#pragma once
#include "afxdialogex.h"


// CSectionInsertDlg 对话框

class CSectionInsertDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionInsertDlg)

public:
	CSectionInsertDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSectionInsertDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_SECTION_INSERT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_addAsmEdt;
	CEdit m_extendAsmEdt;
	CString m_addName;
	CString m_addMisc;
	CString m_addRva;
	CString m_addSize;
	CString m_addFa;
	CString m_addChars;
	afx_msg void OnBnClickedAddInsert();
	CString m_extendName;
	CString m_extendMisc;
	CString m_extendRva;
	CString m_extendSize;
	CString m_extendFa;
	CString m_extendChars;
	afx_msg void OnBnClickedExtendInsert();
	virtual BOOL OnInitDialog();
};
