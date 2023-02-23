#pragma once
#include "afxdialogex.h"


// COptionalHeaderDlg 对话框

class COptionalHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptionalHeaderDlg)

public:
	COptionalHeaderDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~COptionalHeaderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_OPTIONAL_HEADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_optionalHeaderList;
	virtual BOOL OnInitDialog();
	void InitList();
	void RenderListData();
	void RefreshListStyle();
};
