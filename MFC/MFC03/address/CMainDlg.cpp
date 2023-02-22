#include "CMainDlg.h"
#include "CAddDlg.h"
#include "CListDlg.h"
#include "CModDlg.h"
#include "CDlgApp.h"
#include "resource.h"

CMainDlg::CMainDlg(CWnd* pParent) : CDialogEx(DLG_MAIN, pParent)
{

}
BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(BTN_ADDCONTACT, &CMainDlg::OnBnClickedAddcontact)
	ON_BN_CLICKED(BTN_MODCONTACT, &CMainDlg::OnBnClickedModcontact)
	ON_BN_CLICKED(BTN_VIEWCONTACT, &CMainDlg::OnBnClickedViewcontact)
END_MESSAGE_MAP()

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


void CMainDlg::OnBnClickedAddcontact()
{
	CAddDlg dlg;
	dlg.DoModal();
}


void CMainDlg::OnBnClickedModcontact()
{
	CModDlg dlg;
	dlg.DoModal();
}


void CMainDlg::OnBnClickedViewcontact()
{
	CListDlg dlg;
	dlg.DoModal();
}
