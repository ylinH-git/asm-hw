// CTlsDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CTlsDlg.h"
#define InsertTlsMember(X, IDX, NEEDNAME) { \
	InsertMember(X, 22, IDX, m_tlsInfo, currentOffset, NEEDNAME); \
	IDX++; \
}

// CTlsDlg 对话框
IMPLEMENT_DYNAMIC(CTlsDlg, CDialogEx)

CTlsDlg::CTlsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_TLS, pParent)
{

}

CTlsDlg::~CTlsDlg()
{
}

void CTlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_TLS_INFO, m_tlsInfo);
	DDX_Control(pDX, LIST_TLS_VAL_LIST, m_tlsValList);
}


BEGIN_MESSAGE_MAP(CTlsDlg, CDialogEx)
END_MESSAGE_MAP()


// CTlsDlg 消息处理程序


BOOL CTlsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_tlsInfo.DeleteAllItems();
	m_tlsValList.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		m_tlsInfo.DeleteColumn(0);

	}

	for (int i = 0; i < 3; i++)
	{
		m_tlsValList.DeleteColumn(0);

	}
	InitTlsInfoList();
	InitTlsValList();
	RenderTlsInfoData();
	RenderTlsValListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTlsDlg::InitTlsInfoList()
{
	m_tlsInfo.SetExtendedStyle(m_tlsInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_tlsInfo.InsertColumn(0, "Member");
	m_tlsInfo.InsertColumn(1, "Offset");
	m_tlsInfo.InsertColumn(2, "Size");
	m_tlsInfo.InsertColumn(3, "Value");
	RefreshTlsInfoStyle();
}

void CTlsDlg::RenderTlsInfoData()
{
	if (theApp.m_tlsDirectory.StartAddressOfRawData) {
		ULONG currentOffset = theApp.m_tlsDirectoryFA;
		int currIdx = 0;
		InsertTlsMember(theApp.m_tlsDirectory.StartAddressOfRawData, currIdx, TRUE);
		InsertTlsMember(theApp.m_tlsDirectory.EndAddressOfRawData, currIdx, TRUE);
		InsertTlsMember(theApp.m_tlsDirectory.AddressOfIndex, currIdx, TRUE);
		InsertTlsMember(theApp.m_tlsDirectory.AddressOfCallBacks, currIdx, TRUE);
		InsertTlsMember(theApp.m_tlsDirectory.SizeOfZeroFill, currIdx, TRUE);
		InsertTlsMember(theApp.m_tlsDirectory.Characteristics, currIdx, TRUE);
		RefreshTlsInfoStyle();
	}
}

void CTlsDlg::RefreshTlsInfoStyle()
{
	m_tlsInfo.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_tlsInfo.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_tlsInfo.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_tlsInfo.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}

void CTlsDlg::InitTlsValList()
{
	m_tlsValList.SetExtendedStyle(m_tlsValList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_tlsValList.InsertColumn(0, "Offset");
	m_tlsValList.InsertColumn(1, "Size");
	m_tlsValList.InsertColumn(2, "Value");
	RefreshTlsInfoStyle();
}

void CTlsDlg::RenderTlsValListData()
{
	CString sValue;
	ULONG currOffset = theApp.m_tlsValFA;
	if (theApp.m_tlsValList) {
		for (int i = 0; i < theApp.m_tlsValListNum; i++)
		{
			sValue.Format("%08X", currOffset);
			m_tlsValList.InsertItem(i, sValue);
			m_tlsValList.SetItemText(i, 1, "DWORD");
			sValue.Format("%08X", theApp.m_tlsValList[i]);
			m_tlsValList.SetItemText(i, 2, sValue);
			currOffset += sizeof(DWORD);
		}
	}
	RefreshTlsValListStyle();
}

void CTlsDlg::RefreshTlsValListStyle()
{
	m_tlsValList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_tlsValList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_tlsValList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}
