// CDataDirecotriesDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CDataDirectoriesDlg.h"


// CDataDirecotriesDlg 对话框

IMPLEMENT_DYNAMIC(CDataDirectoriesDlg, CDialogEx)

CDataDirectoriesDlg::CDataDirectoriesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_DATA_DIRECTORIES, pParent)
{

}

CDataDirectoriesDlg::~CDataDirectoriesDlg()
{
}

void CDataDirectoriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_DATA_DIRECTORIES, m_dataDirectoriesList);
}


BEGIN_MESSAGE_MAP(CDataDirectoriesDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataDirectoriesDlg 消息处理程序


BOOL CDataDirectoriesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
