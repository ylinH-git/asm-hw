#include "CDlgApp.h"
#include "CMainDlg.h"

CDlgApp theApp;

BOOL CDlgApp::InitInstance()
{
	CMainDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
    return 0;
}