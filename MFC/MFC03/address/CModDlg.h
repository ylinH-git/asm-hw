#pragma once
#include <afxdialogex.h>
#include "resource.h"
class CModDlg :
    public CDialogEx
{
public:
    CModDlg();	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_MODCONTACT };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
    CString m_searchKey;
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedModsearch();
    CString m_name;
    CString m_tel;
    int m_gender;
private:
    int m_findIdx;
public:
    afx_msg void OnBnClickedModify();
};
