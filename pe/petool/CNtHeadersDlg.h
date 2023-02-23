#pragma once
#include "afxdialogex.h"


// CNtHeadersDlg 对话框

class CNtHeadersDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNtHeadersDlg)

public:
	CNtHeadersDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNtHeadersDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_NT_HEADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ntHeaderList;
	virtual BOOL OnInitDialog();
	void InitList();
	void RenderListData();
	void RefreshListStyle();
};
