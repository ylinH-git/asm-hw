// CNtHeadersDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CNtHeadersDlg.h"
#define InsertNtMember(X, IDX, NEEDNAME) { \
	int offset = theApp.m_ntOffset; \
	InsertMember(X, 18, IDX, m_ntHeaderList, offset, NEEDNAME); \
	IDX++; \
}

// CNtHeadersDlg 对话框

IMPLEMENT_DYNAMIC(CNtHeadersDlg, CDialogEx)

CNtHeadersDlg::CNtHeadersDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_NT_HEADER, pParent)
{

}

CNtHeadersDlg::~CNtHeadersDlg()
{
}

void CNtHeadersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_NT_HEADER, m_ntHeaderList);
}


BEGIN_MESSAGE_MAP(CNtHeadersDlg, CDialogEx)
END_MESSAGE_MAP()


// CNtHeadersDlg 消息处理程序


BOOL CNtHeadersDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ntHeaderList.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		m_ntHeaderList.DeleteColumn(0);

	}
	InitList();
	RenderListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNtHeadersDlg::InitList()
{
	m_ntHeaderList.SetExtendedStyle(m_ntHeaderList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_ntHeaderList.InsertColumn(0, "Member");
	m_ntHeaderList.InsertColumn(1, "Offset");
	m_ntHeaderList.InsertColumn(2, "Size");
	m_ntHeaderList.InsertColumn(3, "Value");
	RefreshListStyle();
}

void CNtHeadersDlg::RenderListData()
{
	int currIdx = 0;
	InsertNtMember(theApp.m_ntHeader.Signature, currIdx, TRUE);
	RefreshListStyle();
}

void CNtHeadersDlg::RefreshListStyle()
{
	m_ntHeaderList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ntHeaderList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_ntHeaderList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_ntHeaderList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}
