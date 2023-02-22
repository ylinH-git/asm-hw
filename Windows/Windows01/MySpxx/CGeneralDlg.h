#pragma once


// CGeneralDlg 对话框

class CGeneralDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGeneralDlg)

public:
	CGeneralDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGeneralDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_GENERAL_ATTR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeWinTitle();
	CString m_title;
	CString m_handle;
	CString m_process;
	CString m_rect;
	CString m_normalRect;
	CString m_clientRect;
	CString m_insHandle;
	CString m_menuHandle;
	CString m_userData;
	CString m_winByte;
};
