// CFileHeaderDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CFileHeaderDlg.h"
#define InsertFileHMember(X, IDX, NEEDNAME) { \
	InsertMember(X, 20, IDX, m_fileHeaderList, currentOffset, NEEDNAME); \
	IDX++; \
}

// CFileHeaderDlg 对话框

IMPLEMENT_DYNAMIC(CFileHeaderDlg, CDialogEx)

CFileHeaderDlg::CFileHeaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_FILE_HEADER, pParent)
{

}

CFileHeaderDlg::~CFileHeaderDlg()
{
}

void CFileHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_FILE_HEADER, m_fileHeaderList);
}


BEGIN_MESSAGE_MAP(CFileHeaderDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileHeaderDlg 消息处理程序


BOOL CFileHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_fileHeaderList.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		m_fileHeaderList.DeleteColumn(0);

	}
	InitList();
	RenderListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFileHeaderDlg::InitList()
{
	m_fileHeaderList.SetExtendedStyle(m_fileHeaderList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_fileHeaderList.InsertColumn(0, "Member");
	m_fileHeaderList.InsertColumn(1, "Offset");
	m_fileHeaderList.InsertColumn(2, "Size");
	m_fileHeaderList.InsertColumn(3, "Value");
	RefreshListStyle();
}

void CFileHeaderDlg::RenderListData()
{
	ULONG currentOffset = theApp.m_ntOffset + sizeof(theApp.m_ntHeader.Signature);
	int currIdx = 0;
	InsertFileHMember(theApp.m_fileHeader.Machine, currIdx, TRUE);
	InsertFileHMember(theApp.m_fileHeader.NumberOfSections, currIdx, TRUE);
	InsertFileHMember(theApp.m_fileHeader.TimeDateStamp, currIdx, TRUE);
	InsertFileHMember(theApp.m_fileHeader.PointerToSymbolTable, currIdx, TRUE);
	InsertFileHMember(theApp.m_fileHeader.NumberOfSymbols, currIdx, TRUE);
	InsertFileHMember(theApp.m_fileHeader.SizeOfOptionalHeader, currIdx, TRUE);
	InsertFileHMember(theApp.m_fileHeader.Characteristics, currIdx, TRUE);
	RefreshListStyle();
}

void CFileHeaderDlg::RefreshListStyle()
{
	m_fileHeaderList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_fileHeaderList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_fileHeaderList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_fileHeaderList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}
