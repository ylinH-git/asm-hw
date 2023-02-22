// CAddStrDlg.cpp: 实现文件
//

#include "pch.h"
#include "RegManager.h"
#include "CAddStrDlg.h"
#include "afxdialogex.h"


// CAddStrDlg 对话框

IMPLEMENT_DYNAMIC(CAddStrDlg, CDialogEx)

CAddStrDlg::CAddStrDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ADSTRVALUE, pParent)
	, m_name(_T(""))
	, m_value(_T(""))
{

}

CAddStrDlg::~CAddStrDlg()
{
}

void CAddStrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_NAME, m_name);
	DDX_Text(pDX, EDT_VALUE, m_value);
}


BEGIN_MESSAGE_MAP(CAddStrDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddStrDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddStrDlg 消息处理程序


void CAddStrDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CAddStrDlg::SetDlg(CString name, CString value)
{
	m_name = name;
	m_value = value;
}
