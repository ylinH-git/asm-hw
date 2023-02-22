#pragma once
#include <afxdialogex.h>
#include "resource.h"
class CMyRegDlg :
    public CDialogEx
{
public:
    CMyRegDlg();	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_REG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
    CString m_lgName;
    CString m_pwd;
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedConfirm();
};

