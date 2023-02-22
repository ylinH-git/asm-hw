
// clientDlg.h: 头文件
//

#pragma once
#include "Proto.h"


// CclientDlg 对话框
class CclientDlg : public CDialogEx
{
// 构造
public:
	CclientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
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
	CString m_chatInfo;
	CString m_loginName;
	CListBox m_contactList;
	CString m_speakInfo;
	BOOL m_isLogin = FALSE;
	SOCKET m_socket;
	HANDLE m_hThread;
	sockaddr_in m_siServer = {};
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedSendPrivate();
protected:
	afx_msg LRESULT OnCustomUpdate(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
