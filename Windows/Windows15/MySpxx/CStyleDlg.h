#pragma once


// CStyleDlg 对话框

class CStyleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStyleDlg)

public:
	CStyleDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CStyleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_STYLE_ATTR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_winStyleStr;
	CString m_exStyleStr;
	CListBox m_winStyleList;
	CListBox m_exStyleList;
};
