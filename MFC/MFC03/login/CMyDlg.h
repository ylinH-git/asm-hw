#pragma once

#include <afxdialogex.h>
#include "resource.h"
#include "CMyEdit.h"

class CMyDlg :
    public CDialogEx
{
public:
    CMyDlg(CWnd * pParent = nullptr);	// ��׼���캯��

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_LOGIN };
#endif
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedLogin();
    afx_msg void OnBnClickedReg();
public:
    CMyEdit m_cLgNEdit;
    CMyEdit m_cPwdEdit;
};

