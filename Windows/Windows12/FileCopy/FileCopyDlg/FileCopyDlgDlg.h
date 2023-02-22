
// FileCopyDlgDlg.h: 头文件
//

#pragma once
class CFileCopyDlgDlg;
struct tagThreadsParams {
	CFileCopyDlgDlg* dlg;
	int idx;
	LONGLONG offset;
	LONGLONG endPos;
};

// CFileCopyDlgDlg 对话框
class CFileCopyDlgDlg : public CDialogEx
{
// 构造
public:
	CFileCopyDlgDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILECOPYDLG_DIALOG };
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
	afx_msg void OnBnClickedStart();
	CProgressCtrl m_progress;
	HANDLE m_hThreads[4] = {NULL, NULL, NULL, NULL};
	int m_hThreadsNum = 4;
	tagThreadsParams* m_hThreadsParamsList[4] = {};
	LONGLONG m_hThreadsBytesReaded[4] = {};
	CStatic m_progressText;
};

void ClearFileHandle();
