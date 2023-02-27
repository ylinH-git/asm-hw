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
	ON_BN_CLICKED(BTN_VA, &CAddrConvertDlg::OnBnClickedVa)
	ON_BN_CLICKED(BTN_RVA, &CAddrConvertDlg::OnBnClickedRva)
	ON_BN_CLICKED(BTN_FA, &CAddrConvertDlg::OnBnClickedFa)
END_MESSAGE_MAP()


// CAddrConvertDlg 消息处理程序


void CAddrConvertDlg::OnBnClickedVa()
{
	UpdateData(TRUE);
	ULONG offset = theApp.GetHex(m_vaStr.GetString());
	ULONG rva = theApp.GetVAtoRVA(offset);
	ULONG fa = theApp.GetRVAtoFA(rva);
	m_rvaStr.Format("%08X", rva);
	m_faStr.Format("%08X", fa);
	UpdateData(FALSE);
}


void CAddrConvertDlg::OnBnClickedRva()
{
	UpdateData(TRUE);
	ULONG offset = theApp.GetHex(m_rvaStr.GetString());
	ULONG va = theApp.GetRVAtoVA(offset);
	ULONG fa = theApp.GetRVAtoFA(offset);
	m_vaStr.Format("%08X", va);
	m_faStr.Format("%08X", fa);
	UpdateData(FALSE);
}


void CAddrConvertDlg::OnBnClickedFa()
{
	UpdateData(TRUE);
	ULONG offset = theApp.GetHex(m_faStr.GetString());
	ULONG rva = theApp.GetFAtoRVA(offset);
	ULONG va = theApp.GetRVAtoFA(rva);
	m_rvaStr.Format("%08X", rva);
	m_vaStr.Format("%08X", va);
	UpdateData(FALSE);
}
