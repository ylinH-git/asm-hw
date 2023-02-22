// CAddDwordDlg.cpp: 实现文件
//

#include "pch.h"
#include "RegManager.h"
#include "CAddDwordDlg.h"
#include "afxdialogex.h"


// CAddDwordDlg 对话框

IMPLEMENT_DYNAMIC(CAddDwordDlg, CDialogEx)

CAddDwordDlg::CAddDwordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ADDDWORDVALUE, pParent)
	, m_name(_T(""))
	, m_value(_T(""))
{

}

CAddDwordDlg::~CAddDwordDlg()
{
}

void CAddDwordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_NAME, m_name);
	DDX_Text(pDX, EDT_VALUE, m_value);
}


BEGIN_MESSAGE_MAP(CAddDwordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddDwordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddDwordDlg 消息处理程序


void CAddDwordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CAddDwordDlg::SetDlg(CString name, CString value)
{
	m_name = name;
	m_value = value;
}