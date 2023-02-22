// CThreadDlg.cpp: 实现文件
//

#include "pch.h"
#include "ProcessViewer.h"
#include "CThreadDlg.h"
#include "afxdialogex.h"


// CThreadDlg 对话框

IMPLEMENT_DYNAMIC(CThreadDlg, CDialogEx)

CThreadDlg::CThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREAD_DIALOG, pParent)
{

}

CThreadDlg::~CThreadDlg()
{
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_THREAD, m_threadList);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreadDlg 消息处理程序
