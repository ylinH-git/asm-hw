#pragma once
#include "afxdialogex.h"


// CImportDirectoryDlg 对话框

class CImportDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDirectoryDlg)

public:
	CImportDirectoryDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImportDirectoryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_IMPORT_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_funcList;
	CListCtrl m_moduleList;
	IMAGE_IMPORT_BY_NAME* m_OFTList = nullptr;
	ULONG* m_oftRVAs = nullptr;
	ULONG* m_ftRVAs = nullptr;
	int* m_funcsLen = nullptr;
	ULONG* m_oftFAs = nullptr;
	ULONG* m_ftFAs = nullptr;
	void InitModuleList();
	void RenderModuleListData();
	void RefreshModuleListStyle();
	void InitFuncList();
	void RenderFuncListData(int nIdx);
	void RefreshFuncListStyle();
	void ClearMem();
	void ClearFuncMem();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListModuleList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListFuncList(NMHDR* pNMHDR, LRESULT* pResult);
};
