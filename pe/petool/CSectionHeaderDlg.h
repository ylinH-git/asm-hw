#pragma once
#include "afxdialogex.h"


// CSectionHeaderDlg 对话框

class CSectionHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionHeaderDlg)

public:
	CSectionHeaderDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSectionHeaderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_SECTION_HEADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_sectionHeaderList;
	void InitList();
	void RenderListData();
	void RefreshListStyle();
	virtual BOOL OnInitDialog();
//	afx_msg void OnClickListSectionHeader(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnItemchangedListSectionHeader(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListSectionHeader(NMHDR* pNMHDR, LRESULT* pResult);
};
