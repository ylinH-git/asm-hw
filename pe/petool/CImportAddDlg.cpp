// CImportAddDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CImportAddDlg.h"


// CImportAddDlg 对话框

IMPLEMENT_DYNAMIC(CImportAddDlg, CDialogEx)

CImportAddDlg::CImportAddDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_IMPORT_ADD, pParent)
	, m_filePath(_T(""))
	, m_funcName(_T(""))
{

}

CImportAddDlg::~CImportAddDlg()
{
}

void CImportAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, BROWSE_DLL, m_filePath);
	DDX_Text(pDX, EDT_FUN, m_funcName);
	DDX_Control(pDX, LIST_DLL, m_dllList);
	DDX_Control(pDX, LIST_FUNC, m_funcLIst);
}


BEGIN_MESSAGE_MAP(CImportAddDlg, CDialogEx)
	ON_BN_CLICKED(BTN_DLL, &CImportAddDlg::OnBnClickedDll)
	ON_BN_CLICKED(BTN_FUNC, &CImportAddDlg::OnBnClickedFunc)
	ON_BN_CLICKED(BTN_INSERT, &CImportAddDlg::OnBnClickedInsert)
	ON_LBN_DBLCLK(LIST_DLL, &CImportAddDlg::OnDblclkListDll)
END_MESSAGE_MAP()


// CImportAddDlg 消息处理程序


void CImportAddDlg::OnBnClickedDll()
{
	UpdateData(TRUE);
	if (m_filePath.Right(4) == ".dll")
	{
		int nIdx = m_dllList.GetCount();
		CString dllName = m_filePath.Right(m_filePath.GetLength() - 1 - m_filePath.ReverseFind('\\'));
		m_dllNames[nIdx] = dllName;
		m_dllPaths[nIdx] = m_filePath;
		m_dllList.AddString(dllName);
	}
	else
	{
		AfxMessageBox("不是有效的dll文件");
	}
	UpdateData(FALSE);
}


void CImportAddDlg::OnBnClickedFunc()
{
	UpdateData(TRUE);
	int curDllIdx = m_dllList.GetCurSel();
	int nIdx = m_funcLIst.GetCount();
	m_funcNames[curDllIdx][nIdx] = m_funcName;
	m_funcLIst.AddString(m_funcName);
	UpdateData(FALSE);
}


void CImportAddDlg::OnBnClickedInsert()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CImportAddDlg::OnDblclkListDll()
{
	// TODO: 在此添加控件通知处理程序代码
	int curDllIdx = m_dllList.GetCurSel();
	int total = m_funcLIst.GetCount();
	for (int i = 0; i < total; i++)
	{
		m_funcLIst.DeleteString(0);
	}
	for (int i = 0; i < 500; i++)
	{
		if (m_funcNames[curDllIdx][i].IsEmpty())
		{
			break;
		}
		m_funcLIst.AddString(m_funcNames[curDllIdx][i]);
	}
}
