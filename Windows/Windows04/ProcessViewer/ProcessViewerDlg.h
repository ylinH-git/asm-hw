
// ProcessViewerDlg.h: 头文件
//

#pragma once


// CProcessViewerDlg 对话框
class CProcessViewerDlg : public CDialogEx
{
// 构造
public:
	CProcessViewerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void SetColumnWidthAuto();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_processList;
private:
	BOOL GetProcessList();
public:
	afx_msg void OnDblclkListProcess(NMHDR* pNMHDR, LRESULT* pResult);
};
