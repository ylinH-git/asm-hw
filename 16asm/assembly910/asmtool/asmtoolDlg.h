
// asmtoolDlg.h: 头文件
//

#pragma once


// CasmtoolDlg 对话框
class CasmtoolDlg : public CDialogEx
{
// 构造
public:
	CasmtoolDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CasmtoolDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASMTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCEditBrowseCtrl m_fileEdit;
	afx_msg void OnBnClickedImport();
	char* m_asmBuf = nullptr;
	long m_fileSize = 0;
	afx_msg void OnBnClickedDisasm();
	CString m_disasmText;
};
