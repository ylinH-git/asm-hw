// CImportDirectoryDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CImportDirectoryDlg.h"


// CImportDirctoryDlg 对话框

IMPLEMENT_DYNAMIC(CImportDirectoryDlg, CDialogEx)

CImportDirectoryDlg::CImportDirectoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_IMPORT_DIRECTORY, pParent)
{

}

CImportDirectoryDlg::~CImportDirectoryDlg()
{
	ClearMem();
	ClearFuncMem();
}

void CImportDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_FUNC_LIST, m_funcList);
	DDX_Control(pDX, LIST_MODULE_LIST, m_moduleList);
}

void CImportDirectoryDlg::InitModuleList()
{
	m_moduleList.SetExtendedStyle(m_moduleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_moduleList.InsertColumn(0, "Module Name");
	m_moduleList.InsertColumn(1, "Imports");
	m_moduleList.InsertColumn(2, "OFTs");
	m_moduleList.InsertColumn(3, "TimeDateStamp");
	m_moduleList.InsertColumn(4, "ForwarderChain");
	m_moduleList.InsertColumn(5, "Name RVA");
	m_moduleList.InsertColumn(6, "FTs(IAT)");
	m_moduleList.InsertItem(0, "");
	m_moduleList.SetItemText(0, 1, "");
	m_moduleList.SetItemText(0, 2, "");
	m_moduleList.SetItemText(0, 3, "");
	m_moduleList.SetItemText(0, 4, "");
	m_moduleList.SetItemText(0, 5, "");
	m_moduleList.SetItemText(0, 6, "");


	m_moduleList.InsertItem(1, "szAnsi");
	m_moduleList.SetItemText(1, 1, "(nFunctions)");
	m_moduleList.SetItemText(1, 2, "Dword");
	m_moduleList.SetItemText(1, 3, "Dword");
	m_moduleList.SetItemText(1, 4, "Dword");
	m_moduleList.SetItemText(1, 5, "Dword");
	m_moduleList.SetItemText(1, 6, "Dword");
	RefreshModuleListStyle();
}

void CImportDirectoryDlg::RenderModuleListData()
{
	ClearMem();
	int currSectionOffset = theApp.m_importDescriptorFA;
	m_funcsLen = new int[theApp.m_importDescriptorLen];
	m_oftFAs = new ULONG[theApp.m_importDescriptorLen];
	m_ftFAs = new ULONG[theApp.m_importDescriptorLen];
	for (int i = 0; i < theApp.m_importDescriptorLen; i++)
	{
		char sName[255] = {};
		int row = i + 2;
		theApp.m_importDllNameFAs[i] = theApp.GetRVAtoFA(theApp.m_importDescriptors[i].Name);
		fseek(theApp.m_pFile, theApp.m_importDllNameFAs[i], SEEK_SET);
		char curChar = fgetc(theApp.m_pFile);
		int sIdx = 0;
		while (curChar != 0)
		{
			sName[sIdx] = curChar;
			sIdx++;
			curChar = fgetc(theApp.m_pFile);
		}
		sName[sIdx] = 0;
		m_moduleList.InsertItem(row, sName);


		CString sValue;
		m_oftFAs[i] = theApp.GetRVAtoFA(theApp.m_importDescriptors[i].OriginalFirstThunk);
		m_ftFAs[i] = theApp.GetRVAtoFA(theApp.m_importDescriptors[i].FirstThunk);
		m_funcsLen[i] = 0;
		fseek(theApp.m_pFile, m_oftFAs[i], SEEK_SET);
		int oftSize = sizeof(ULONG);
		char* tempOft = new char[oftSize];
		char* allZero = new char[oftSize];
		RtlZeroMemory(allZero, oftSize);
		while (1)
		{
			fread(tempOft, 1, oftSize, theApp.m_pFile);
			if (memcmp(tempOft, allZero, oftSize) == 0)
			{
				break;
			}
			m_funcsLen[i]++;
		}
		sValue.Format("%d", m_funcsLen[i]);
		m_moduleList.SetItemText(row, 1, sValue);

		sValue.Format("%08X", *(DWORD*)&theApp.m_importDescriptors[i].OriginalFirstThunk);
		m_moduleList.SetItemText(row, 2, sValue);
		m_moduleList.SetItemData(row, currSectionOffset);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_importDescriptors[i].TimeDateStamp);
		m_moduleList.SetItemText(row, 3, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_importDescriptors[i].ForwarderChain);
		m_moduleList.SetItemText(row, 4, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_importDescriptors[i].Name);
		m_moduleList.SetItemText(row, 5, sValue);
		currSectionOffset += sizeof(DWORD);

		sValue.Format("%08X", *(DWORD*)&theApp.m_importDescriptors[i].FirstThunk);
		m_moduleList.SetItemText(row, 6, sValue);
		currSectionOffset += sizeof(DWORD);
	}
	RefreshModuleListStyle();
}

void CImportDirectoryDlg::RefreshModuleListStyle()
{
	m_moduleList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleList.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);
	m_moduleList.SetColumnWidth(6, LVSCW_AUTOSIZE_USEHEADER);
}

void CImportDirectoryDlg::InitFuncList()
{
	m_funcList.SetExtendedStyle(m_moduleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_funcList.InsertColumn(0, "OFTs");
	m_funcList.InsertColumn(1, "FTs(IAT)");
	m_funcList.InsertColumn(2, "Hint");
	m_funcList.InsertColumn(3, "Name");
	m_funcList.InsertItem(0, "");
	m_funcList.SetItemText(0, 1, "");
	m_funcList.SetItemText(0, 2, "");
	m_funcList.SetItemText(0, 3, "");


	m_funcList.InsertItem(1, "Dword");
	m_funcList.SetItemText(1, 1, "Dword");
	m_funcList.SetItemText(1, 2, "WORD");
	m_funcList.SetItemText(1, 3, "szAnsi");
	RefreshFuncListStyle();
}

void CImportDirectoryDlg::RenderFuncListData(int nIdx)
{
	ClearFuncMem();
	m_OFTList = new IMAGE_IMPORT_BY_NAME[m_funcsLen[nIdx]];
	m_oftRVAs = new ULONG[m_funcsLen[nIdx]];
	m_ftRVAs = new ULONG[m_funcsLen[nIdx]];
	fseek(theApp.m_pFile, m_oftFAs[nIdx], SEEK_SET);
	fread(m_oftRVAs, 1, sizeof(ULONG) * m_funcsLen[nIdx], theApp.m_pFile);

	fseek(theApp.m_pFile, m_oftFAs[nIdx], SEEK_SET);
	fread(m_oftRVAs, 1, sizeof(ULONG) * m_funcsLen[nIdx], theApp.m_pFile);

	fseek(theApp.m_pFile, m_ftFAs[nIdx], SEEK_SET);
	fread(m_ftRVAs, 1, sizeof(ULONG) * m_funcsLen[nIdx], theApp.m_pFile);

	ULONG nameStrucOffset = theApp.GetRVAtoFA(m_oftRVAs[0]);
	fseek(theApp.m_pFile, nameStrucOffset, SEEK_SET);
	fread(m_OFTList, 1, sizeof(IMAGE_IMPORT_BY_NAME) * m_funcsLen[nIdx], theApp.m_pFile);

	for (int i = 0; i < m_funcsLen[nIdx]; i++)
	{
		int row = i + 2;
		CString sValue;
		sValue.Format("%08X", m_oftRVAs[i]);
		m_funcList.InsertItem(row, sValue);

		sValue.Format("%08X", m_ftRVAs[i]);
		m_funcList.SetItemText(row, 1, sValue);

		if (m_oftRVAs[i] > 0x80000000)
		{
			m_funcList.SetItemText(row, 2, "N/A");
			ULONG ordinal = (m_oftRVAs[i] << 16) >> 16;
			sValue.Format("%08X", ordinal);
			m_funcList.SetItemText(row, 3, "Ordinal: " + sValue);
		}
		else
		{
			sValue.Format("%04X", m_OFTList[i].Hint);
			m_funcList.SetItemText(row, 2, sValue);


			char sName[255] = {};
			ULONG nameOffset = theApp.GetRVAtoFA(m_oftRVAs[i]);
			fseek(theApp.m_pFile, nameOffset + 2, SEEK_SET);
			char curChar = fgetc(theApp.m_pFile);
			int sIdx = 0;
			while (curChar != 0)
			{
				sName[sIdx] = curChar;
				sIdx++;
				curChar = fgetc(theApp.m_pFile);
			}
			sName[sIdx] = 0;
			m_funcList.SetItemText(row, 3, sName);
		}

		
	}
	RefreshFuncListStyle();
}

void CImportDirectoryDlg::RefreshFuncListStyle()
{
	m_funcList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_funcList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_funcList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_funcList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}

void CImportDirectoryDlg::ClearMem()
{
	if (m_funcsLen) {
		delete[] m_funcsLen;
		m_funcsLen = nullptr;
	}

	if (m_oftFAs) {
		delete[] m_oftFAs;
		m_oftFAs = nullptr;
	}

	if (m_ftFAs) {
		delete[] m_ftFAs;
		m_ftFAs = nullptr;
	}
}

void CImportDirectoryDlg::ClearFuncMem()
{
	if (m_OFTList) {
		delete[] m_OFTList;
		m_OFTList = nullptr;
	}

	if (m_oftRVAs) {
		delete[] m_oftRVAs;
		m_oftRVAs = nullptr;
	}

	if (m_ftRVAs) {
		delete[] m_ftRVAs;
		m_ftRVAs = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CImportDirectoryDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, LIST_MODULE_LIST, &CImportDirectoryDlg::OnClickListModuleList)
	ON_NOTIFY(NM_CLICK, LIST_FUNC_LIST, &CImportDirectoryDlg::OnClickListFuncList)
END_MESSAGE_MAP()


// CImportDirectoryDlg 消息处理程序


BOOL CImportDirectoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_moduleList.DeleteAllItems();
	m_funcList.DeleteAllItems();
	for (int i = 0; i < 7; i++)
	{
		m_moduleList.DeleteColumn(0);
	}
	for (int i = 0; i < 4; i++)
	{
		m_funcList.DeleteColumn(0);

	}
	InitModuleList();
	RenderModuleListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CImportDirectoryDlg::OnClickListModuleList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int currItem = m_moduleList.GetSelectionMark();
	if (currItem < 2)
	{
		return;
	}
	CString sValue;
	sValue.Format("%08X", theApp.m_importDllNameFAs[currItem - 2]);
	m_moduleList.SetItemText(0, 0, sValue);

	m_moduleList.SetItemText(0, 1, "N/A");

	int currOffset = m_moduleList.GetItemData(currItem);
	sValue.Format("%08X", currOffset);
	m_moduleList.SetItemText(0, 2, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_moduleList.SetItemText(0, 3, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_moduleList.SetItemText(0, 4, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_moduleList.SetItemText(0, 5, sValue);

	sValue.Format("%08X", currOffset += sizeof(DWORD));
	m_moduleList.SetItemText(0, 6, sValue);

	RefreshModuleListStyle();

	m_funcList.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		m_funcList.DeleteColumn(0);
	}
	InitFuncList();
	RenderFuncListData(currItem - 2);
}


void CImportDirectoryDlg::OnClickListFuncList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int currItem = m_funcList.GetSelectionMark();
	if (currItem < 2)
	{
		return;
	}

	int curIdx = currItem - 2;
	CString sValue;
	ULONG oftOffset = m_oftFAs[0] + curIdx * sizeof(DWORD);
	sValue.Format("%08X", oftOffset);
	m_funcList.SetItemText(0, 0, sValue);

	ULONG ftOffset = m_ftFAs[0] + curIdx * sizeof(DWORD);
	sValue.Format("%08X", ftOffset);
	m_funcList.SetItemText(0, 1, sValue);

	if (m_oftRVAs[curIdx] > 0x80000000)
	{
		m_funcList.SetItemText(0, 2, "N/A");
		m_funcList.SetItemText(0, 3, "N/A");
	}
	else
	{
		ULONG nameOffset = theApp.GetRVAtoFA(m_oftRVAs[curIdx]);
		sValue.Format("%08X", nameOffset);
		m_funcList.SetItemText(0, 2, sValue);
		sValue.Format("%08X", nameOffset + 2);
		m_funcList.SetItemText(0, 3, sValue);
	}

	RefreshFuncListStyle();
}
