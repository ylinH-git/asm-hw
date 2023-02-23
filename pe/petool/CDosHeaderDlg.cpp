// CDosHeaderDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CDosHeaderDlg.h"


// CDosHeaderDlg 对话框

IMPLEMENT_DYNAMIC(CDosHeaderDlg, CDialogEx)

CDosHeaderDlg::CDosHeaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_DOS_HEADER, pParent)
{

}

CDosHeaderDlg::~CDosHeaderDlg()
{
}

void CDosHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_DOS_HEADER, m_dosHeaderList);
}


BEGIN_MESSAGE_MAP(CDosHeaderDlg, CDialogEx)
END_MESSAGE_MAP()


// CDosHeaderDlg 消息处理程序
