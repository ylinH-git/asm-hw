#pragma once


// CMoreDlg 对话框
#include "CModuleDlg.h"
#include "CThreadDlg.h"
#include "CWindowDlg.h"

class CMoreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoreDlg)

public:
	CMoreDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMoreDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MORE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	DWORD m_processId;
public:
	void SetProcessId(DWORD processId);
	DWORD GetProcessId();
	CModuleDlg m_moduleDlg;
	CThreadDlg m_threadDlg;
	CWindowDlg m_windowDlg;
	virtual BOOL OnInitDialog();
	void InitWindowDlg();
	void SetWindowListWidth();
	void InitModuleDlg();
	BOOL ListProcessModules();
	void SetModuleListWidth();
	void InitThreadDlg();
	BOOL ListProcessThreads();
	void SetThreadListWidth();
	CTabCtrl m_tab;
	afx_msg void OnSelchangeMoreTab(NMHDR* pNMHDR, LRESULT* pResult);
};
