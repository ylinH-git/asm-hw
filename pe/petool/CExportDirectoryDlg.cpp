// CExportDirectoryDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CExportDirectoryDlg.h"


// CExportDirectoryDlg 对话框
#define InsertExportMember(X, IDX, NEEDNAME) { \
	InsertMember(X, 25, IDX, m_exportDirectory, currentOffset, NEEDNAME); \
	IDX++; \
}

IMPLEMENT_DYNAMIC(CExportDirectoryDlg, CDialogEx)

CExportDirectoryDlg::CExportDirectoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_EXPORT_DIRECTORY, pParent)
{

}

CExportDirectoryDlg::~CExportDirectoryDlg()
{
}

void CExportDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_EXPORT_DIRECTORY, m_exportDirectory);
	DDX_Control(pDX, LIST_FUNC_LIST, m_funcList);
}


BEGIN_MESSAGE_MAP(CExportDirectoryDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, LIST_FUNC_LIST, &CExportDirectoryDlg::OnClickListFuncList)
END_MESSAGE_MAP()


// CExportDirectoryDlg 消息处理程序


BOOL CExportDirectoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (theApp.m_exportDescriptorFA) {
		m_exportDirectory.DeleteAllItems();
		for (int i = 0; i < 4; i++)
		{
			m_exportDirectory.DeleteColumn(0);

		}
		InitExportDirectoryList();
		RenderExportDirectoryListData();

		m_funcList.DeleteAllItems();
		for (int i = 0; i < 5; i++)
		{
			m_funcList.DeleteColumn(0);

		}
		InitFuncList();
		RenderFuncListData();
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CExportDirectoryDlg::InitExportDirectoryList()
{
	m_exportDirectory.SetExtendedStyle(m_exportDirectory.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_exportDirectory.InsertColumn(0, "Member");
	m_exportDirectory.InsertColumn(1, "Offset");
	m_exportDirectory.InsertColumn(2, "Size");
	m_exportDirectory.InsertColumn(3, "Value");
	RefreshExportDirectoryListStyle();
}

void CExportDirectoryDlg::RenderExportDirectoryListData()
{
	ULONG currentOffset = theApp.m_exportDescriptorFA;
	int currIdx = 0;
	InsertExportMember(theApp.m_exportDirectory.Characteristics, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.TimeDateStamp, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.MajorVersion, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.MinorVersion, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.Name, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.Base, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.NumberOfFunctions, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.NumberOfNames, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.AddressOfFunctions, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.AddressOfNames, currIdx, TRUE);
	InsertExportMember(theApp.m_exportDirectory.AddressOfNameOrdinals, currIdx, TRUE);
	RefreshExportDirectoryListStyle();
}

void CExportDirectoryDlg::RefreshExportDirectoryListStyle()
{
	m_exportDirectory.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_exportDirectory.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_exportDirectory.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_exportDirectory.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}

void CExportDirectoryDlg::InitFuncList()
{
	m_funcList.SetExtendedStyle(m_funcList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_funcList.InsertColumn(0, "Ordinal");
	m_funcList.InsertColumn(1, "Function RVA");
	m_funcList.InsertColumn(2, "Name Ordinal");
	m_funcList.InsertColumn(3, "Name RVA");
	m_funcList.InsertColumn(4, "Name");
	m_funcList.InsertItem(0, "");
	m_funcList.SetItemText(0, 1, "");
	m_funcList.SetItemText(0, 2, "");
	m_funcList.SetItemText(0, 3, "");
	m_funcList.SetItemText(0, 4, "");


	m_funcList.InsertItem(1, "(nFunctions)");
	m_funcList.SetItemText(1, 1, "Dword");
	m_funcList.SetItemText(1, 2, "Word");
	m_funcList.SetItemText(1, 3, "Dword");
	m_funcList.SetItemText(1, 4, "szAnsi");
	RefreshFuncListStyle();
}

void CExportDirectoryDlg::RenderFuncListData()
{
	CString sValue;
	int row = 2;
	for (int i = 0; i < theApp.m_exportDirectory.NumberOfFunctions; i++)
	{
		if (theApp.m_exportFuncList[i].m_funcRva != 0)
		{
			sValue.Format("%08X", theApp.m_exportFuncList[i].m_ordinal);
			m_funcList.InsertItem(row, sValue);
			sValue.Format("%08X", theApp.m_exportFuncList[i].m_funcRva);
			m_funcList.SetItemText(row, 1, sValue);
			if (theApp.m_exportFuncList[i].m_nameOrdinal != 0)
			{
				sValue.Format("%04X", theApp.m_exportFuncList[i].m_nameOrdinal);
				m_funcList.SetItemText(row, 2, sValue);
				sValue.Format("%08X", theApp.m_exportFuncList[i].m_nameRva);
				m_funcList.SetItemText(row, 3, sValue);
				m_funcList.SetItemText(row, 4, theApp.m_exportFuncList[i].m_name);
			}
			else
			{
				m_funcList.SetItemText(row, 2, "N/A");
				m_funcList.SetItemText(row, 3, "N/A");
				m_funcList.SetItemText(row, 4, "N/A");
			}
			m_funcList.SetItemData(row, i);
			row++;
		}
	}
	RefreshFuncListStyle();
}

void CExportDirectoryDlg::RefreshFuncListStyle()
{
	m_funcList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_funcList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_funcList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_funcList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_funcList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
}


void CExportDirectoryDlg::OnClickListFuncList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int currItem = m_funcList.GetSelectionMark();
	if (currItem < 2)
	{
		return;
	}
	int currIdx = m_funcList.GetItemData(currItem);
	CString sValue;
	m_funcList.SetItemText(0, 0,"N/A");
	sValue.Format("%08X", theApp.m_exportFuncList[currIdx].m_funcRvaOffset);
	m_funcList.SetItemText(0, 1, sValue);
	if (theApp.m_exportFuncList[currIdx].m_nameOrdinal != 0)
	{
		sValue.Format("%08X", theApp.m_exportFuncList[currIdx].m_nameOrdinalOffset);
		m_funcList.SetItemText(0, 2, sValue);
		sValue.Format("%08X", theApp.m_exportFuncList[currIdx].m_nameOffset);
		m_funcList.SetItemText(0, 3, sValue);
		sValue.Format("%08X", theApp.m_exportFuncList[currIdx].m_nameFa);
		m_funcList.SetItemText(0, 4, sValue);
	}
	else
	{
		m_funcList.SetItemText(0, 2, "N/A");
		m_funcList.SetItemText(0, 3, "N/A");
		m_funcList.SetItemText(0, 4, "N/A");
	}

	RefreshFuncListStyle();
}
