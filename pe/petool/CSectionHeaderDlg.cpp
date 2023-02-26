// CSectionHeaderDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CSectionHeaderDlg.h"


// CSectionHeaderDlg 对话框

IMPLEMENT_DYNAMIC(CSectionHeaderDlg, CDialogEx)

CSectionHeaderDlg::CSectionHeaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_SECTION_HEADER, pParent)
{

}

CSectionHeaderDlg::~CSectionHeaderDlg()
{
}

void CSectionHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_SECTION_HEADER, m_sectionHeaderList);
}

void CSectionHeaderDlg::InitList()
{
	m_sectionHeaderList.SetExtendedStyle(m_sectionHeaderList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_sectionHeaderList.InsertColumn(0, "Name");
	m_sectionHeaderList.InsertColumn(1, "Virtual Size");
	m_sectionHeaderList.InsertColumn(2, "Virtual Address");
	m_sectionHeaderList.InsertColumn(3, "Raw Size");
	m_sectionHeaderList.InsertColumn(4, "Raw Address");
	m_sectionHeaderList.InsertColumn(5, "Reloc Address");
	m_sectionHeaderList.InsertColumn(6, "Linenumbers");
	m_sectionHeaderList.InsertColumn(7, "Relocations Number");
	m_sectionHeaderList.InsertColumn(8, "Linenumbers Number");
	m_sectionHeaderList.InsertColumn(9, "Characteristics");
	m_sectionHeaderList.InsertItem(0, "");
	m_sectionHeaderList.SetItemText(0, 1, "");
	m_sectionHeaderList.SetItemText(0, 2, "");
	m_sectionHeaderList.SetItemText(0, 3, "");
	m_sectionHeaderList.SetItemText(0, 4, "");
	m_sectionHeaderList.SetItemText(0, 5, "");
	m_sectionHeaderList.SetItemText(0, 6, "");
	m_sectionHeaderList.SetItemText(0, 7, "");
	m_sectionHeaderList.SetItemText(0, 8, "");
	m_sectionHeaderList.SetItemText(0, 9, "");


	m_sectionHeaderList.InsertItem(1, "Byte[8]");
	m_sectionHeaderList.SetItemText(1, 1, "Dword");
	m_sectionHeaderList.SetItemText(1, 2, "Dword");
	m_sectionHeaderList.SetItemText(1, 3, "Dword");
	m_sectionHeaderList.SetItemText(1, 4, "Dword");
	m_sectionHeaderList.SetItemText(1, 5, "Dword");
	m_sectionHeaderList.SetItemText(1, 6, "Dword");
	m_sectionHeaderList.SetItemText(1, 7, "Word");
	m_sectionHeaderList.SetItemText(1, 8, "Word");
	m_sectionHeaderList.SetItemText(1, 9, "Dword");
	RefreshListStyle();
}

void CSectionHeaderDlg::RenderListData()
{
	int currSectionOffset = theApp.m_sectionHeaderOffset;
	for (int i = 0; i < theApp.m_sectionHeaderLen; i++)
	{
		int row = i + 2;
		char* sName = new char[9];
		RtlZeroMemory(sName, 9);
		RtlCopyMemory(sName, &theApp.m_sectionHeaders[i].Name, 8);
		m_sectionHeaderList.InsertItem(row, sName);
		m_sectionHeaderList.SetItemData(row, currSectionOffset);

		CString sValue;
		sValue.Format("%08X", *(DWORD*)&theApp.m_sectionHeaders[i].Misc);
		m_sectionHeaderList.SetItemText(row, 1, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_sectionHeaders[i].VirtualAddress);
		m_sectionHeaderList.SetItemText(row, 2, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_sectionHeaders[i].SizeOfRawData);
		m_sectionHeaderList.SetItemText(row, 3, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_sectionHeaders[i].PointerToRawData);
		m_sectionHeaderList.SetItemText(row, 4, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_sectionHeaders[i].PointerToRelocations);
		m_sectionHeaderList.SetItemText(row, 5, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_sectionHeaders[i].PointerToLinenumbers);
		m_sectionHeaderList.SetItemText(row, 6, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%04X", *(WORD*)&theApp.m_sectionHeaders[i].NumberOfRelocations);
		m_sectionHeaderList.SetItemText(row, 7, sValue);
		currSectionOffset += sizeof(WORD);

		sValue.Format("%04X", *(WORD*)&theApp.m_sectionHeaders[i].NumberOfLinenumbers);
		m_sectionHeaderList.SetItemText(row, 8, sValue);
		currSectionOffset += sizeof(WORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_sectionHeaders[i].Characteristics);
		m_sectionHeaderList.SetItemText(row, 9, sValue);
		currSectionOffset += sizeof(DWORD);

		delete[] sName;
	}
	RefreshListStyle();
}

void CSectionHeaderDlg::RefreshListStyle()
{
	m_sectionHeaderList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(6, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(7, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(8, LVSCW_AUTOSIZE_USEHEADER);
	m_sectionHeaderList.SetColumnWidth(9, LVSCW_AUTOSIZE_USEHEADER);
}


BEGIN_MESSAGE_MAP(CSectionHeaderDlg, CDialogEx)
//	ON_NOTIFY(NM_CLICK, LIST_SECTION_HEADER, &CSectionHeaderDlg::OnClickListSectionHeader)
//ON_NOTIFY(LVN_ITEMCHANGED, LIST_SECTION_HEADER, &CSectionHeaderDlg::OnItemchangedListSectionHeader)
ON_NOTIFY(NM_CLICK, LIST_SECTION_HEADER, &CSectionHeaderDlg::OnClickListSectionHeader)
END_MESSAGE_MAP()


// CSectionHeaderDlg 消息处理程序


BOOL CSectionHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_sectionHeaderList.DeleteAllItems();
	for (int i = 0; i < 10; i++)
	{
		m_sectionHeaderList.DeleteColumn(0);

	}
	InitList();
	RenderListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


//void CSectionHeaderDlg::OnClickListSectionHeader(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


//void CSectionHeaderDlg::OnItemchangedListSectionHeader(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//
//}


void CSectionHeaderDlg::OnClickListSectionHeader(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int currItem = m_sectionHeaderList.GetSelectionMark();
	if (currItem < 2)
	{
		return;
	}
	int currOffset = m_sectionHeaderList.GetItemData(currItem);
	CString sValue;
	sValue.Format("%08X", currOffset);
	m_sectionHeaderList.SetItemText(0, 0, sValue);

	sValue.Format("%08X", currOffset += 8);
	m_sectionHeaderList.SetItemText(0, 1, sValue);


	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_sectionHeaderList.SetItemText(0, 2, sValue);


	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_sectionHeaderList.SetItemText(0, 3, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_sectionHeaderList.SetItemText(0, 4, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_sectionHeaderList.SetItemText(0, 5, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_sectionHeaderList.SetItemText(0, 6, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_sectionHeaderList.SetItemText(0, 7, sValue);

	sValue.Format("%08X", currOffset += sizeof(WORD));
	m_sectionHeaderList.SetItemText(0, 8, sValue);

	sValue.Format("%08X", currOffset += sizeof(WORD));
	m_sectionHeaderList.SetItemText(0, 9, sValue);
	RefreshListStyle();
}
