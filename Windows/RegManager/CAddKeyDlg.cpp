// CAddKeyDlg.cpp: 实现文件
//

#include "pch.h"
#include "RegManager.h"
#include "CAddKeyDlg.h"
#include "afxdialogex.h"


// CAddKeyDlg 对话框

IMPLEMENT_DYNAMIC(CAddKeyDlg, CDialogEx)

CAddKeyDlg::CAddKeyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ADDKEY, pParent)
	, m_name(_T(""))
{

}

CAddKeyDlg::~CAddKeyDlg()
{
}

void CAddKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_NAME, m_name);
}


BEGIN_MESSAGE_MAP(CAddKeyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddKeyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddKeyDlg 消息处理程序


void CAddKeyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
