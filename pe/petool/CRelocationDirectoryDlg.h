#pragma once
#include "afxdialogex.h"


// CRelocationDirectoryDlg 对话框

class CRelocationDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocationDirectoryDlg)

public:
	CRelocationDirectoryDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRelocationDirectoryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_RELOCATION_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_baseRelocList;
	CListCtrl m_relocAddrList;
	void InitRelocList();
	void RenderRelocListData();
	void RefreshRelocListStyle();
	void InitRelocAddrList();
	void RenderRelocAddrListData(int nIdx);
	void RefreshRelocAddrListStyle();
	afx_msg void OnClickListRelocAddr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListBaseRelocations(NMHDR* pNMHDR, LRESULT* pResult);
};
