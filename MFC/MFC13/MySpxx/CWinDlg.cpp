// CWinDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CWinDlg.h"
#include "afxdialogex.h"


// CWinDlg 对话框

IMPLEMENT_DYNAMIC(CWinDlg, CDialogEx)

CWinDlg::CWinDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_WIN_ATTR, pParent)
	, m_nextHandle(_T(""))
	, m_prevHandle(_T(""))
	, m_parentHandle(_T(""))
	, m_firstHandle(_T(""))
	, m_ownerHandle(_T(""))
	, m_nextTitle(_T(""))
	, m_prevTitle(_T(""))
	, m_parentTitle(_T(""))
	, m_firstTitle(_T(""))
	, m_ownerTitle(_T(""))
{

}

CWinDlg::~CWinDlg()
{
}

void CWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, STA_NEXT_HANDLE, m_nextHandle);
	DDX_Text(pDX, STA_PREV_HANDLE, m_prevHandle);
	DDX_Text(pDX, STA_PARENT_HANDLE, m_parentHandle);
	DDX_Text(pDX, STA_FIRST_HANDLE, m_firstHandle);
	DDX_Text(pDX, STA_OWNER_HANDLE, m_ownerHandle);
	DDX_Text(pDX, EDI_NEXT_TITLE, m_nextTitle);
	DDX_Text(pDX, EDI_PREV_TITLE, m_prevTitle);
	DDX_Text(pDX, EDI_PARENT_TITLE, m_parentTitle);
	DDX_Text(pDX, EDI_FIRST_TITLE, m_firstTitle);
	DDX_Text(pDX, EDI_OWNER_TITLE, m_ownerTitle);
}


BEGIN_MESSAGE_MAP(CWinDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinDlg 消息处理程序
