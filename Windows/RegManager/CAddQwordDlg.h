#pragma once


// CAddQwordDlg 对话框

class CAddQwordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddQwordDlg)

public:
	CAddQwordDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddQwordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_ADDQWORDVALUE };
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
