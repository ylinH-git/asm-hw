#include "CMyDlg.h"
#include "CDlgApp.h"
#include "CMyRegDlg.h"
#include "resource.h"

CMyDlg::CMyDlg(CWnd* pParent) : CDialogEx(DLG_LOGIN, pParent)
{

}
BEGIN_MESSAGE_MAP(CMyDlg, CDialogEx)
	ON_BN_CLICKED(BTN_LOGIN, &CMyDlg::OnBnClickedLogin)
	ON_BN_CLICKED(BTN_REG, &CMyDlg::OnBnClickedReg)
END_MESSAGE_MAP()


void CMyDlg::OnBnClickedLogin()
{
	if (g_pwd.IsEmpty() || g_loginname.IsEmpty())
	{
		AfxMessageBox("����ע���ٵ�¼");
	}
	else
	{
		CString pwd;
		CString loginname;
		//MFC��ʽ
		GetDlgItemText(ED_LOGINNAME, loginname);
		GetDlgItemText(ED_PWD, pwd);
		if (loginname == g_loginname && pwd == g_pwd)
		{
			AfxMessageBox("��¼�ɹ�");
		}
		else
		{
			AfxMessageBox("�˺Ż��������");
		}
	}

}


void CMyDlg::OnBnClickedReg()
{
	CMyRegDlg regDlg;
	regDlg.DoModal();
}
