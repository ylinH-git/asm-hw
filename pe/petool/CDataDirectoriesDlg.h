#pragma once
#include "afxdialogex.h"


// CDataDirectoriesDlg 对话框

class CDataDirectoriesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDirectoriesDlg)

public:
	CDataDirectoriesDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDataDirectoriesDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_DATA_DIRECTORIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_dataDirectoriesList;
	virtual BOOL OnInitDialog();
};
