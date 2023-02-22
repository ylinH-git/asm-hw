// CGeneralDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CGeneralDlg.h"
#include "afxdialogex.h"


// CGeneralDlg 对话框

IMPLEMENT_DYNAMIC(CGeneralDlg, CDialogEx)

CGeneralDlg::CGeneralDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_GENERAL_ATTR, pParent)
	, m_title(_T(""))
	, m_handle(_T(""))
	, m_process(_T(""))
	, m_rect(_T(""))
	, m_normalRect(_T(""))
	, m_clientRect(_T(""))
	, m_insHandle(_T(""))
	, m_menuHandle(_T(""))
	, m_userData(_T(""))
	, m_winByte(_T(""))
{

}

CGeneralDlg::~CGeneralDlg()
{
}

void CGeneralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDI_WIN_TITLE, m_title);
	DDX_Text(pDX, EDI_WIN_HANDLE, m_handle);
	DDX_Text(pDX, EDI_WIN_PROCESS, m_process);
	DDX_Text(pDX, EDI_WIN_RECT, m_rect);
	DDX_Text(pDX, EDI_WIN_NORMAL_RECT, m_normalRect);
	DDX_Text(pDX, EDI_WIN_CLIENT_RECT, m_clientRect);
	DDX_Text(pDX, EDI_WIN_INS_HANDLE, m_insHandle);
	DDX_Text(pDX, EDI_WIN_MENU_HENDLE, m_menuHandle);
	DDX_Text(pDX, EDI_USER_DATA, m_userData);
	DDX_Text(pDX, EDI_WIN_BYTE, m_winByte);
}


BEGIN_MESSAGE_MAP(CGeneralDlg, CDialogEx)
END_MESSAGE_MAP()


// CGeneralDlg 消息处理程序

