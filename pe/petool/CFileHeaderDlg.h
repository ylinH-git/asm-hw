#pragma once
#include "afxdialogex.h"


// CFileHeaderDlg 对话框

class CFileHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileHeaderDlg)

public:
	CFileHeaderDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFileHeaderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_FILE_HEADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_fileHeaderList;
	virtual BOOL OnInitDialog();
	void InitList();
	void RenderListData();
	void RefreshListStyle();
};
