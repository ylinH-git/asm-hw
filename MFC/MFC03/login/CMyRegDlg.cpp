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
	DDX_Control(pDX, ED_REG_LOGINNAME, m_regLgNEdit);
	DDX_Control(pDX, ED_REG_PWD, m_cPwdEdit);
}
BEGIN_MESSAGE_MAP(CMyRegDlg, CDialogEx)
	ON_BN_CLICKED(BTN_CONFIRM, &CMyRegDlg::OnBnClickedConfirm)
END_MESSAGE_MAP()


void CMyRegDlg::OnBnClickedConfirm()
{
	UpdateData(TRUE);

	const char* upChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* lowerChar = "abcdefghijklmnopqrstuvwxyz";
	const char* numChar = "1234567890";
	if (m_pwd.FindOneOf(upChar) != -1 && m_pwd.FindOneOf(lowerChar) != -1 && m_pwd.FindOneOf(numChar) != -1)
	{
		g_pwd = m_pwd;
		g_loginname = m_lgName;
		EndDialog(0);
	}
	else
	{
		AfxMessageBox("密码必须同时包含大小写字母和数字");
	}
}
