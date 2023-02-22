// CStyleDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CStyleDlg.h"
#include "afxdialogex.h"


// CStyleDlg 对话框

IMPLEMENT_DYNAMIC(CStyleDlg, CDialogEx)

CStyleDlg::CStyleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_STYLE_ATTR, pParent)
	, m_winStyleStr(_T(""))
	, m_exStyleStr(_T(""))
{

}

CStyleDlg::~CStyleDlg()
{
}

void CStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDI_WIN_STYLE, m_winStyleStr);
	DDX_Text(pDX, EDI_EX_STYLE, m_exStyleStr);
	DDX_Control(pDX, LIST_WIN_STYLE, m_winStyleList);
	DDX_Control(pDX, LIST_EX_STYLE, m_exStyleList);
}


BEGIN_MESSAGE_MAP(CStyleDlg, CDialogEx)
END_MESSAGE_MAP()


// CStyleDlg 消息处理程序
