// CClassDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CClassDlg.h"
#include "afxdialogex.h"


// CClassDlg 对话框

IMPLEMENT_DYNAMIC(CClassDlg, CDialogEx)

CClassDlg::CClassDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_CLASS_ATTR, pParent)
	, m_className(_T(""))
	, m_classStyle(_T(""))
	, m_classByte(_T(""))
	, m_insHandle(_T(""))
	, m_winProcess(_T(""))
	, m_menuName(_T(""))
	, m_iconHandle(_T(""))
	, m_cursorHandle(_T(""))
	, m_classAtom(_T(""))
	, m_bgPen(_T(""))
	, m_winByte(_T(""))
{

}

CClassDlg::~CClassDlg()
{
}

void CClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDI_CLASSNAME, m_className);
	DDX_Text(pDX, EDI_CLASS_STYLE, m_classStyle);
	DDX_Control(pDX, COMBO_CLASS_STYLE, m_classStyleBox);
	DDX_Text(pDX, EDI_CLASS_BYTE, m_classByte);
	DDX_Control(pDX, COMBO_CLASS_BYTE, m_classByteBox);
	DDX_Text(pDX, EDI_INS_HANDLE, m_insHandle);
	DDX_Text(pDX, EDI_WIN_PROCESS, m_winProcess);
	DDX_Text(pDX, EDI_MENU_NAME, m_menuName);
	DDX_Text(pDX, EDI_ICON_HANDLE, m_iconHandle);
	DDX_Text(pDX, EDI_CURSOR_HANDLE, m_cursorHandle);
	DDX_Text(pDX, EDI_CLASS_ATOM, m_classAtom);
	DDX_Text(pDX, EDI_BG_PEN, m_bgPen);
	DDX_Text(pDX, EDI_WIN_BYTE, m_winByte);
}


BEGIN_MESSAGE_MAP(CClassDlg, CDialogEx)

END_MESSAGE_MAP()


// CClassDlg 消息处理程序
