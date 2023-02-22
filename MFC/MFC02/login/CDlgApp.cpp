#include "CDlgApp.h"
#include "CMyDlg.h"

CDlgApp theApp;
CString g_pwd;
CString g_loginname;

BOOL CDlgApp::InitInstance()
{
	CMyDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
    return 0;
}