#pragma once


// CThreadDlg 对话框

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CThreadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREAD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_threadList;
};
