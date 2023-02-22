// CAddQwordDlg.cpp: 实现文件
//

#include "pch.h"
#include "RegManager.h"
#include "CAddQwordDlg.h"
#include "afxdialogex.h"


// CAddQwordDlg 对话框

IMPLEMENT_DYNAMIC(CAddQwordDlg, CDialogEx)

CAddQwordDlg::CAddQwordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ADDQWORDVALUE, pParent)
	, m_name(_T(""))
	, m_value(_T(""))
{

}

CAddQwordDlg::~CAddQwordDlg()
{
}

void CAddQwordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_NAME, m_name);
	DDX_Text(pDX, EDT_VALUE, m_value);
}


BEGIN_MESSAGE_MAP(CAddQwordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddQwordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddQwordDlg 消息处理程序


void CAddQwordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CAddQwordDlg::SetDlg(CString name, CString value)
{
	m_name = name;
	m_value = value;
}

