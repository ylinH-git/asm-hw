// CRelocationDirectoryDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CRelocationDirectoryDlg.h"


// CRelocationDirectoryDlg 对话框

IMPLEMENT_DYNAMIC(CRelocationDirectoryDlg, CDialogEx)

CRelocationDirectoryDlg::CRelocationDirectoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_RELOCATION_DIRECTORY, pParent)
{

}

CRelocationDirectoryDlg::~CRelocationDirectoryDlg()
{
}

void CRelocationDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_BASE_RELOCATIONS, m_baseRelocList);
	DDX_Control(pDX, LIST_RELOC_ADDR, m_relocAddrList);
}


BEGIN_MESSAGE_MAP(CRelocationDirectoryDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, LIST_RELOC_ADDR, &CRelocationDirectoryDlg::OnClickListRelocAddr)
	ON_NOTIFY(NM_CLICK, LIST_BASE_RELOCATIONS, &CRelocationDirectoryDlg::OnClickListBaseRelocations)
END_MESSAGE_MAP()


// CRelocationDirectoryDlg 消息处理程序


BOOL CRelocationDirectoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_baseRelocList.DeleteAllItems();
	m_relocAddrList.DeleteAllItems();
	for (int i = 0; i < 3; i++)
	{
		m_baseRelocList.DeleteColumn(0);
	}
	for (int i = 0; i < 3; i++)
	{
		m_relocAddrList.DeleteColumn(0);

	}
	InitRelocList();
	RenderRelocListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRelocationDirectoryDlg::InitRelocList()
{
	m_baseRelocList.SetExtendedStyle(m_baseRelocList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_baseRelocList.InsertColumn(0, "RVA");
	m_baseRelocList.InsertColumn(1, "Size Of Block");
	m_baseRelocList.InsertColumn(2, "Items");
	m_baseRelocList.InsertItem(0, "");
	m_baseRelocList.SetItemText(0, 1, "");
	m_baseRelocList.SetItemText(0, 2, "");


	m_baseRelocList.InsertItem(1, "DWORD");
	m_baseRelocList.SetItemText(1, 1, "DWORD");
	m_baseRelocList.SetItemText(1, 2, "N/A");
	RefreshRelocListStyle();
}

void CRelocationDirectoryDlg::RenderRelocListData()
{
	if (theApp.m_baseRelocation) {
		IMAGE_BASE_RELOCATION* currBase = theApp.m_baseRelocation;
		DWORD currBaseFA = theApp.m_relocationDirectoryFA;
		DWORD lastAddr = (DWORD)currBase + theApp.m_dataDirectoris[5].Size;
		int row = 2;
		CString sValue;
		while ((DWORD)currBase < lastAddr)
		{
			sValue.Format("%08X", currBase->VirtualAddress);
			m_baseRelocList.InsertItem(row, sValue);
			sValue.Format("%08X", currBase->SizeOfBlock);
			m_baseRelocList.SetItemText(row, 1, sValue);
			int num = (currBase->SizeOfBlock - sizeof(currBase->VirtualAddress) - sizeof(currBase->SizeOfBlock)) / sizeof(WORD);
			sValue.Format("%d", num);
			m_baseRelocList.SetItemText(row, 2, sValue);
			m_baseRelocList.SetItemData(row, currBaseFA);
			currBaseFA = (DWORD)currBase + currBase->SizeOfBlock;
			currBase = (IMAGE_BASE_RELOCATION*)((DWORD)currBase + currBase->SizeOfBlock);
			row += 1;
		}
	}
	RefreshRelocListStyle();
}

void CRelocationDirectoryDlg::RefreshRelocListStyle()
{
	m_baseRelocList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_baseRelocList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_baseRelocList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}

void CRelocationDirectoryDlg::InitRelocAddrList()
{
	m_relocAddrList.SetExtendedStyle(m_relocAddrList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_relocAddrList.InsertColumn(0, "Item");
	m_relocAddrList.InsertColumn(1, "RVA");
	m_relocAddrList.InsertColumn(2, "TYPE");
	m_relocAddrList.InsertItem(0, "");
	m_relocAddrList.SetItemText(0, 1, "");
	m_relocAddrList.SetItemText(0, 2, "");


	m_relocAddrList.InsertItem(1, "WORD");
	m_relocAddrList.SetItemText(1, 1, "N/A");
	m_relocAddrList.SetItemText(1, 2, "N/A");
	RefreshRelocAddrListStyle();
}

void CRelocationDirectoryDlg::RenderRelocAddrListData(int nIdx)
{
	IMAGE_BASE_RELOCATION* currBase = theApp.m_baseRelocation;
	CString addrLenStr = m_baseRelocList.GetItemText(nIdx, 2);
	CString addrBaseStr = m_baseRelocList.GetItemText(nIdx, 0);
	ULONG addrFA = m_baseRelocList.GetItemData(nIdx) + sizeof(DWORD) * 2;
	int addrLen = atoi(addrLenStr.GetString());
	int addrBase = theApp.GetHex(addrBaseStr.GetString());
	WORD* currAddr = (WORD*)((DWORD*)theApp.m_baseRelocation + 2);
	CString sValue;
	for (int i = 0; i < addrLen; i++)
	{
		int row = 2 + i;
		WORD currItem = *currAddr;
		sValue.Format("%04X", currItem);
		m_relocAddrList.InsertItem(row, sValue);

		DWORD currRva = currItem & 0x00000fff;
		currRva += addrBase;
		sValue.Format("%08X", currRva);
		m_relocAddrList.SetItemText(row, 1, sValue);

		int currType = (currItem & 0x0000f000) >> 12;
		switch (currType) 
		{
		case IMAGE_REL_BASED_ABSOLUTE:
			m_relocAddrList.SetItemText(row, 2, "ABSOLUTE");
			break;
		case IMAGE_REL_BASED_HIGH:
			m_relocAddrList.SetItemText(row, 2, "HIGH");
			break;
		case IMAGE_REL_BASED_LOW:
			m_relocAddrList.SetItemText(row, 2, "LOW");
			break;
		case IMAGE_REL_BASED_HIGHLOW:
			m_relocAddrList.SetItemText(row, 2, "HIGHLOW");
			break;
		case IMAGE_REL_BASED_HIGHADJ:
			m_relocAddrList.SetItemText(row, 2, "HIGHADJ");
			break;
		case IMAGE_REL_BASED_MACHINE_SPECIFIC_5:
			m_relocAddrList.SetItemText(row, 2, "MACHINE_SPECIFIC_5");
			break;
		case IMAGE_REL_BASED_RESERVED:
			m_relocAddrList.SetItemText(row, 2, "RESERVED");
			break;
		case IMAGE_REL_BASED_MACHINE_SPECIFIC_7:
			m_relocAddrList.SetItemText(row, 2, "MACHINE_SPECIFIC_7");
			break;
		case IMAGE_REL_BASED_MACHINE_SPECIFIC_8:
			m_relocAddrList.SetItemText(row, 2, "MACHINE_SPECIFIC_8");
			break;
		case IMAGE_REL_BASED_MACHINE_SPECIFIC_9:
			m_relocAddrList.SetItemText(row, 2, "MACHINE_SPECIFIC_9");
			break;
		case IMAGE_REL_BASED_DIR64:
			m_relocAddrList.SetItemText(row, 2, "DIR64");
			break;
		}
		m_relocAddrList.SetItemData(row, addrFA);
		currAddr++;
		addrFA += sizeof(WORD);
	}
	RefreshRelocAddrListStyle();
}

void CRelocationDirectoryDlg::RefreshRelocAddrListStyle()
{
	m_relocAddrList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_relocAddrList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_relocAddrList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}


void CRelocationDirectoryDlg::OnClickListRelocAddr(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int currItem = m_relocAddrList.GetSelectionMark();
	if (currItem < 2)
	{
		return;
	}
	CString sValue;
	int currOffset = m_relocAddrList.GetItemData(currItem);
	sValue.Format("%08X", currOffset);
	m_relocAddrList.SetItemText(0, 0, sValue);
	m_relocAddrList.SetItemText(0, 1, "N/A");
	m_relocAddrList.SetItemText(0, 2, "N/A");

	RefreshRelocAddrListStyle();

}


void CRelocationDirectoryDlg::OnClickListBaseRelocations(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int currItem = m_baseRelocList.GetSelectionMark();
	if (currItem < 2)
	{
		return;
	}
	CString sValue;
	int currOffset = m_baseRelocList.GetItemData(currItem);
	sValue.Format("%08X", currOffset);
	m_baseRelocList.SetItemText(0, 0, sValue);
	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_baseRelocList.SetItemText(0, 1, sValue);
	m_baseRelocList.SetItemText(0, 2, "N/A");

	RefreshRelocListStyle();

	m_relocAddrList.DeleteAllItems();
	for (int i = 0; i < 3; i++)
	{
		m_relocAddrList.DeleteColumn(0);
	}
	InitRelocAddrList();
	RenderRelocAddrListData(currItem);
}
