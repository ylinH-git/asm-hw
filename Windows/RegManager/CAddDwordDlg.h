#pragma once


// CAddDwordDlg 对话框

class CAddDwordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDwordDlg)

public:
	CAddDwordDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddDwordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_ADDDWORDVALUE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_value;
	afx_msg void OnBnClickedOk();
	void SetDlg(CString name, CString value);
};
