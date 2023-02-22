#pragma once


// CHookWin 对话框

class CHookWin : public CDialogEx
{
	DECLARE_DYNAMIC(CHookWin)

public:
	CHookWin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHookWin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_HOOK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_result;
	DWORD m_threadId;
	DWORD m_processId;
	HWND m_curHwnd;
	HHOOK m_hHook;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedClose();
};
