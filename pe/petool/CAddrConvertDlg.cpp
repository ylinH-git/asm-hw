// CAddrConvertDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CAddrConvertDlg.h"


// CAddrConvertDlg 对话框

IMPLEMENT_DYNAMIC(CAddrConvertDlg, CDialogEx)

CAddrConvertDlg::CAddrConvertDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ADDR_CONVERT, pParent)
	, m_vaStr(_T(""))
	, m_rvaStr(_T(""))
	, m_faStr(_T(""))
{

}

CAddrConvertDlg::~CAddrConvertDlg()
{
}

void CAddrConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_VA, m_vaStr);
	DDX_Text(pDX, EDT_RVA, m_rvaStr);
	DDX_Text(pDX, EDT_FA, m_faStr);
}


BEGIN_MESSAGE_MAP(CAddrConvertDlg, CDialogEx)
END_MESSAGE_MAP()


// CAddrConvertDlg 消息处理程序
