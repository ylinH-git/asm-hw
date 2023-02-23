#pragma once
#include "afxdialogex.h"


// CDosHeaderDlg 对话框

class CDosHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDosHeaderDlg)

public:
	CDosHeaderDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDosHeaderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_DOS_HEADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_dosHeaderList;
};
