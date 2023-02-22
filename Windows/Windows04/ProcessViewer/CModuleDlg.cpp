// CModuleDlg.cpp: 实现文件
//

#include "pch.h"
#include "ProcessViewer.h"
#include "CModuleDlg.h"
#include "afxdialogex.h"


// CModuleDlg 对话框

IMPLEMENT_DYNAMIC(CModuleDlg, CDialogEx)

CModuleDlg::CModuleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODULE_DIALOG, pParent)
{

}

CModuleDlg::~CModuleDlg()
{
}

void CModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_MODULE, m_moduleList);
}


BEGIN_MESSAGE_MAP(CModuleDlg, CDialogEx)
END_MESSAGE_MAP()


// CModuleDlg 消息处理程序
