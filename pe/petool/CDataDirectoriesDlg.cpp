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
	m_dataDirectoriesList.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		m_dataDirectoriesList.DeleteColumn(0);

	}
	InitList();
	RenderListData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDataDirectoriesDlg::InitList()
{
	m_dataDirectoriesList.SetExtendedStyle(m_dataDirectoriesList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_dataDirectoriesList.InsertColumn(0, "Member");
	m_dataDirectoriesList.InsertColumn(1, "Offset");
	m_dataDirectoriesList.InsertColumn(2, "Size");
	m_dataDirectoriesList.InsertColumn(3, "Value");
	RefreshListStyle();
}

void CDataDirectoriesDlg::RenderListData()
{
	int currOffset = theApp.m_dataDirectoryOffset;
	m_dataDirectoriesList.InsertItem(0, "Export Directory RVA");
	m_dataDirectoriesList.InsertItem(1, "Export Directory Size");
	m_dataDirectoriesList.InsertItem(2, "Import Directory RVA");
	m_dataDirectoriesList.InsertItem(3, "Import Directory Size");
	m_dataDirectoriesList.InsertItem(4, "Resource Directory RVA");
	m_dataDirectoriesList.InsertItem(5, "Resource Directory Size");
	m_dataDirectoriesList.InsertItem(6, "Exception Directory RVA");
	m_dataDirectoriesList.InsertItem(7, "Exception Directory Size");
	m_dataDirectoriesList.InsertItem(8, "Security Directory RVA");
	m_dataDirectoriesList.InsertItem(9, "Security Directory Size");
	m_dataDirectoriesList.InsertItem(10, "Relocation Directory RVA");
	m_dataDirectoriesList.InsertItem(11, "Relocation Directory Size");
	m_dataDirectoriesList.InsertItem(12, "Debug Directory RVA");
	m_dataDirectoriesList.InsertItem(13, "Debug Directory Size");
	m_dataDirectoriesList.InsertItem(14, "Architecture Directory RVA");
	m_dataDirectoriesList.InsertItem(15, "Architecture Directory Size");
	m_dataDirectoriesList.InsertItem(16, "Reserved");
	m_dataDirectoriesList.InsertItem(17, "Reserved");
	m_dataDirectoriesList.InsertItem(18, "TLS Directory RVA");
	m_dataDirectoriesList.InsertItem(19, "TLS Directory Size");
	m_dataDirectoriesList.InsertItem(20, "Configuration Directory RVA");
	m_dataDirectoriesList.InsertItem(21, "Configuration Directory Size");
	m_dataDirectoriesList.InsertItem(22, "Bound Import Directory RVA");
	m_dataDirectoriesList.InsertItem(23, "Bound Import Directory Size");
	m_dataDirectoriesList.InsertItem(24, "Import Address Directory RVA");
	m_dataDirectoriesList.InsertItem(25, "Import Address  Directory Size");
	m_dataDirectoriesList.InsertItem(26, "Delay Import Directory RVA");
	m_dataDirectoriesList.InsertItem(27, "Delay Import Directory Size");
	m_dataDirectoriesList.InsertItem(28, ".NET MetaData Directory RVA");
	m_dataDirectoriesList.InsertItem(29, ".NET MetaData Directory Size");
	int row = 0;
	for (int i = 0; i < theApp.m_dataDirectoryLen - 1; i++)
	{
		CString sOffset; 
		sOffset.Format("%08X", currOffset);
		m_dataDirectoriesList.SetItemText(row, 1, sOffset);
		m_dataDirectoriesList.SetItemText(row, 2, "DWORD");
		CString sAddr;
		sAddr.Format("%08X", theApp.m_dataDirectoris[i].VirtualAddress);
		m_dataDirectoriesList.SetItemText(row++, 3, sAddr);
		currOffset += sizeof(DWORD);
		sOffset.Format("%08X", currOffset);
		m_dataDirectoriesList.SetItemText(row, 1, sOffset);
		m_dataDirectoriesList.SetItemText(row, 2, "DWORD");
		CString sSize;
		sSize.Format("%08X", theApp.m_dataDirectoris[i].Size);
		m_dataDirectoriesList.SetItemText(row++, 3, sSize);
	}
	RefreshListStyle();
}

void CDataDirectoriesDlg::RefreshListStyle()
{
	m_dataDirectoriesList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_dataDirectoriesList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_dataDirectoriesList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_dataDirectoriesList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}