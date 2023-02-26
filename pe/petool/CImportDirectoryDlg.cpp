// CImportDirectoryDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CImportDirectoryDlg.h"


// CImportDirctoryDlg 对话框

IMPLEMENT_DYNAMIC(CImportDirectoryDlg, CDialogEx)

CImportDirectoryDlg::CImportDirectoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_IMPORT_DIRECTORY, pParent)
{

}

CImportDirectoryDlg::~CImportDirectoryDlg()
{
}

void CImportDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_FUNC_LIST, m_funcList);
	DDX_Control(pDX, LIST_MODULE_LIST, m_moduleList);
}


BEGIN_MESSAGE_MAP(CImportDirectoryDlg, CDialogEx)
END_MESSAGE_MAP()


// CImportDirectoryDlg 消息处理程序
