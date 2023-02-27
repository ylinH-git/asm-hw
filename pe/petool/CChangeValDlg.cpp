// CChangeValDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CChangeValDlg.h"


// CChangeValDlg 对话框

IMPLEMENT_DYNAMIC(CChangeValDlg, CDialogEx)

CChangeValDlg::CChangeValDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_CHANGE_VAL, pParent)
	, m_oldVal(_T(""))
	, m_newVal(_T(""))
	, m_fieldName(_T(""))
{

}

CChangeValDlg::~CChangeValDlg()
{
}

void CChangeValDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_OLD, m_oldVal);
	DDX_Text(pDX, EDT_NEW, m_newVal);
	DDX_Text(pDX, EDT_FIELD, m_fieldName);
}


BEGIN_MESSAGE_MAP(CChangeValDlg, CDialogEx)
	ON_BN_CLICKED(BTN_OK, &CChangeValDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangeValDlg 消息处理程序


void CChangeValDlg::OnBnClickedOk()
{
	
}
