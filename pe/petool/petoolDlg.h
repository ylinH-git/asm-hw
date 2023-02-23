﻿
// petoolDlg.h: 头文件
//

#pragma once


// CpetoolDlg 对话框
class CpetoolDlg : public CDialogEx
{
// 构造
public:
	CpetoolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PETOOL_DIALOG };
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

private:
	void CreatePeTree(CString fileName);
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSave();
	bool m_bIsInited;
	CTreeCtrl m_peTree;
	CString m_filePath;
	afx_msg void OnSelchangedTreePe(NMHDR* pNMHDR, LRESULT* pResult);
};
