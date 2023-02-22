#pragma once
#include <afxwin.h>
class CDlgApp :
    public CWinApp
{
public:
    virtual BOOL InitInstance();
};
extern CString g_pwd;
extern CString g_loginname;
