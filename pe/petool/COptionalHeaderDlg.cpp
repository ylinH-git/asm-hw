// COptionalHeaderDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "COptionalHeaderDlg.h"
#define InsertOptionalMember(X, IDX, NEEDNAME) { \
	InsertMember(X, 26, IDX, m_optionalHeaderList, currentOffset, NEEDNAME); \
	IDX++; \
}

// COptionalHeaderDlg 对话框

IMPLEMENT_DYNAMIC(COptionalHeaderDlg, CDialogEx)

COptionalHeaderDlg::COptionalHeaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_OPTIONAL_HEADER, pParent)
{

}

COptionalHeaderDlg::~COptionalHeaderDlg()
{
}

void COptionalHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_OPTIONAL_HEADER, m_optionalHeaderList);
}


BEGIN_MESSAGE_MAP(COptionalHeaderDlg, CDialogEx)
END_MESSAGE_MAP()


// COptionalHeaderDlg 消息处理程序


BOOL COptionalHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_optionalHeaderList.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		m_optionalHeaderList.DeleteColumn(0);

	}
	InitList();
	RenderListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void COptionalHeaderDlg::InitList()
{
	m_optionalHeaderList.SetExtendedStyle(m_optionalHeaderList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_optionalHeaderList.InsertColumn(0, "Member");
	m_optionalHeaderList.InsertColumn(1, "Offset");
	m_optionalHeaderList.InsertColumn(2, "Size");
	m_optionalHeaderList.InsertColumn(3, "Value");
	RefreshListStyle();
}

void COptionalHeaderDlg::RenderListData()
{
	ULONG currentOffset = theApp.m_ntOffset + sizeof(theApp.m_ntHeader.Signature) + sizeof(theApp.m_ntHeader.FileHeader);
	int currIdx = 0;
	
	
	if (theApp.isx86)
	{
		InsertOptionalMember(theApp.m_optional32Header.Magic, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MajorLinkerVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MinorLinkerVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfCode, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfInitializedData, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfUninitializedData, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.AddressOfEntryPoint, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.BaseOfCode, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.BaseOfData, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.ImageBase, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SectionAlignment, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.FileAlignment, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MajorOperatingSystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MinorOperatingSystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MajorImageVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MinorImageVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MajorSubsystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.MinorSubsystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.Win32VersionValue, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfImage, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfHeaders, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.CheckSum, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.Subsystem, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.DllCharacteristics, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfStackReserve, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfStackCommit, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfHeapReserve, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.SizeOfHeapCommit, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.LoaderFlags, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional32Header.NumberOfRvaAndSizes, currIdx, TRUE);
	}
	else
	{
		InsertOptionalMember(theApp.m_optional64Header.Magic, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MajorLinkerVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MinorLinkerVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfCode, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfInitializedData, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfUninitializedData, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.AddressOfEntryPoint, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.BaseOfCode, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.ImageBase, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SectionAlignment, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.FileAlignment, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MajorOperatingSystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MinorOperatingSystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MajorImageVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MinorImageVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MajorSubsystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.MinorSubsystemVersion, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.Win32VersionValue, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfImage, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfHeaders, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.CheckSum, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.Subsystem, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.DllCharacteristics, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfStackReserve, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfStackCommit, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfHeapReserve, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.SizeOfHeapCommit, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.LoaderFlags, currIdx, TRUE);
		InsertOptionalMember(theApp.m_optional64Header.NumberOfRvaAndSizes, currIdx, TRUE);
	}
	RefreshListStyle();
}

void COptionalHeaderDlg::RefreshListStyle()
{
	m_optionalHeaderList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_optionalHeaderList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_optionalHeaderList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_optionalHeaderList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}
