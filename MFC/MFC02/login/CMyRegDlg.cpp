#include "CMyRegDlg.h"
#include "CDlgApp.h"
#include "resource.h"

CMyRegDlg::CMyRegDlg() : CDialogEx(DLG_REG)
, m_lgName(_T(""))
, m_pwd(_T(""))
{

}


void CMyRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ED_REG_LOGINNAME, m_lgName);
	DDX_Text(pDX, ED_REG_PWD, m_pwd);
}
BEGIN_MESSAGE_MAP(CMyRegDlg, CDialogEx)
	ON_BN_CLICKED(BTN_CONFIRM, &CMyRegDlg::OnBnClickedConfirm)
END_MESSAGE_MAP()


void CMyRegDlg::OnBnClickedConfirm()
{
	UpdateData(TRUE);
	g_pwd = m_pwd;
	g_loginname = m_lgName;
	EndDialog(0);
}
