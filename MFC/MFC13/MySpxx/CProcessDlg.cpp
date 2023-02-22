// CProcessDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PROCESS_ATTR, pParent)
	, m_processId(_T(""))
	, m_threadId(_T(""))
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, STA_PROCESS_ID, m_processId);
	DDX_Text(pDX, STA_THREAD_ID, m_threadId);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序
