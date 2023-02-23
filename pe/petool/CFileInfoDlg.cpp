// CFileInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CFileInfoDlg.h"


// CFileInfoDlg 对话框

IMPLEMENT_DYNAMIC(CFileInfoDlg, CDialogEx)

CFileInfoDlg::CFileInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_FILE_INFO, pParent)
{

}

CFileInfoDlg::~CFileInfoDlg()
{
}

void CFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_FILE_MAIN_INFO, m_fileBaseInfo);
	DDX_Control(pDX, LIST_FILE_MORE_INFO, m_fileMoreInfo);
}


BEGIN_MESSAGE_MAP(CFileInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileInfoDlg 消息处理程序


BOOL CFileInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
