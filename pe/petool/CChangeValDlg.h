#pragma once
#include "afxdialogex.h"


// CChangeValDlg 对话框

class CChangeValDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeValDlg)

public:
	CChangeValDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChangeValDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_CHANGE_VAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_oldVal;
	CString m_newVal;
	afx_msg void OnBnClickedOk();
	ULONG m_valOffset;
	int m_valSize;
	CString m_fieldName;
};
