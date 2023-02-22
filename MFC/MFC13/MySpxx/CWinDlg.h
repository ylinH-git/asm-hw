#pragma once


// CWinDlg 对话框

class CWinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWinDlg)

public:
	CWinDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWinDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_WIN_ATTR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_nextHandle;
	CString m_prevHandle;
	CString m_parentHandle;
	CString m_firstHandle;
	CString m_ownerHandle;
	CString m_nextTitle;
	CString m_prevTitle;
	CString m_parentTitle;
	CString m_firstTitle;
	CString m_ownerTitle;
};
