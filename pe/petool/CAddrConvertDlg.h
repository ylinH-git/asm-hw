#pragma once
#include "afxdialogex.h"


// CAddrConvertDlg 对话框

class CAddrConvertDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddrConvertDlg)

public:
	CAddrConvertDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddrConvertDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_ADDR_CONVERT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_vaStr;
	CString m_rvaStr;
	CString m_faStr;
	afx_msg void OnBnClickedVa();
	afx_msg void OnBnClickedRva();
	afx_msg void OnBnClickedFa();
};
