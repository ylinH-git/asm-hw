#pragma once

#include <afxdialogex.h>
#include "resource.h"
class CMyDlg :
    public CDialogEx
{
public:
    CMyDlg(CWnd * pParent = nullptr);	// ��׼���캯��

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_LOGIN };
#endif
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedLogin();
    afx_msg void OnBnClickedReg();
};

