#pragma once
#include <afxdialogex.h>
#include "resource.h"
class CAddDlg :
    public CDialogEx
{
public:
    CAddDlg();	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_ADDCONTACT };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
    DECLARE_MESSAGE_MAP()
    CString m_name;
    CString m_tel;
    int m_gender;
    afx_msg void OnBnClickedAddconfirm();
    afx_msg void OnBnClickedSearch();
};
