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
	UpdateData(TRUE);
	if (m_newVal.IsEmpty()) 
	{
		AfxMessageBox("新值不许为空");
		return;
	}

	fseek(theApp.m_pFile, m_valOffset, SEEK_SET);
	switch (m_valSize)
	{
	case 1:
	{
		BYTE bVal = theApp.GetHex(m_newVal.GetString());
		fwrite(&bVal, 1, sizeof(BYTE), theApp.m_pFile);
		break;
	}
	case 2:
	{
		WORD wVal = theApp.GetHex(m_newVal.GetString());
		fwrite(&wVal, 1, sizeof(WORD), theApp.m_pFile);
		break;
	}
	case 4:
	{
		DWORD dwVal = theApp.GetHex(m_newVal.GetString());
		fwrite(&dwVal, 1, sizeof(DWORD), theApp.m_pFile);
		break;
	}
	case 8:
	{
		ULONGLONG ullVal = theApp.GetHex(m_newVal.GetString());
		fwrite(&ullVal, 1, sizeof(ULONGLONG), theApp.m_pFile);
		break;
	}
	}
	
}
