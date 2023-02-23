#pragma once
#include "afxdialogex.h"


// CFileInfoDlg 对话框

class CFileInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileInfoDlg)

public:
	CFileInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_FILE_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_fileBaseInfo;
	CListCtrl m_fileMoreInfo;
	virtual BOOL OnInitDialog();
};
