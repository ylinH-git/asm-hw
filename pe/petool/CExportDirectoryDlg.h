#pragma once
#include "afxdialogex.h"


// CExportDirectoryDlg 对话框

class CExportDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDirectoryDlg)

public:
	CExportDirectoryDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CExportDirectoryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_EXPORT_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_exportDirectory;
	CListCtrl m_funcList;
	virtual BOOL OnInitDialog();
	void InitExportDirectoryList();
	void RenderExportDirectoryListData();
	void RefreshExportDirectoryListStyle();
	void InitFuncList();
	void RenderFuncListData();
	void RefreshFuncListStyle();
	afx_msg void OnClickListFuncList(NMHDR* pNMHDR, LRESULT* pResult);
};
