#pragma once
#include <afxdialogex.h>
#include "resource.h"
#include "CMyEdit.h"
class CMyRegDlg :
    public CDialogEx
{
public:
    CMyRegDlg();	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_REG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
    CString m_lgName;
    CString m_pwd;
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedConfirm();
    CMyEdit m_regLgNEdit;
    CMyEdit m_cPwdEdit;
};
