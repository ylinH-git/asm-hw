// CWindowDlg.cpp: 实现文件
//

#include "pch.h"
#include "ProcessViewer.h"
#include "CWindowDlg.h"
#include "afxdialogex.h"


// CWindowDlg 对话框

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDOW_DIALOG, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_WINDOW, m_windowList);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowDlg 消息处理程序
