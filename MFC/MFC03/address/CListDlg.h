#pragma once
#include <afxdialogex.h>
#include "resource.h"
#include "CDlgApp.h"
class CListDlg :
    public CDialogEx
{
public:
    CListDlg();	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_SHOWCONTACT };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    virtual BOOL OnInitDialog();
public:
    CString m_searchKey;
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedSearch();
    afx_msg void OnBnClickedDelete();

private:
    void InitList();
    void InsertItemHandler(int nRowIdx, int nColIdx, std::vector<CDlgApp::tagMember>::iterator lt);
    void SearchList();
    void SetRows();
public:
    CListCtrl m_listCtrl;
};
