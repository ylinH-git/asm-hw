#pragma once

#include <afxdialogex.h>
#include "resource.h"

class CMainDlg :
    public CDialogEx
{
public:
    CMainDlg(CWnd * pParent = nullptr);	// 标准构造函数

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_MAIN };
#endif
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedAddcontact();
    afx_msg void OnBnClickedModcontact();
    afx_msg void OnBnClickedViewcontact();
};

