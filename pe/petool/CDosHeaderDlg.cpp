// CDosHeaderDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CDosHeaderDlg.h"
#include "CChangeValDlg.h"

// CDosHeaderDlg 对话框
#define InsertDosMember(X, IDX, NEEDNAME) { \
	InsertMember(X, 22, IDX, m_dosHeaderList, currentOffset, NEEDNAME); \
	IDX++; \
}
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
	ON_NOTIFY(NM_DBLCLK, LIST_DOS_HEADER, &CDosHeaderDlg::OnDblclkListDosHeader)
END_MESSAGE_MAP()


// CDosHeaderDlg 消息处理程序


BOOL CDosHeaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_dosHeaderList.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		m_dosHeaderList.DeleteColumn(0);

	}
	InitList();
	RenderListData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDosHeaderDlg::InitList()
{
	m_dosHeaderList.SetExtendedStyle(m_dosHeaderList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//添加列
	m_dosHeaderList.InsertColumn(0, "Member");
	m_dosHeaderList.InsertColumn(1, "Offset");
	m_dosHeaderList.InsertColumn(2, "Size");
	m_dosHeaderList.InsertColumn(3, "Value");
	RefreshListStyle();
}

void CDosHeaderDlg::RenderListData()
{
	int dosHeaderLen = sizeof(IMAGE_DOS_HEADER);
	ULONG currentOffset = 0x00000000;
	int currIdx = 0;
	InsertDosMember(theApp.m_dosHeaderBuf.e_magic, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_cblp, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_cp, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_crlc, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_cparhdr, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_minalloc, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_maxalloc, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_ss, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_sp, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_csum, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_ip, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_cs, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_lfarlc, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_ovno, currIdx, TRUE);
	for (int i = 0; i < 4; i++)
	{
		InsertDosMember(theApp.m_dosHeaderBuf.e_res[i], currIdx, i == 0);
	}
	
	InsertDosMember(theApp.m_dosHeaderBuf.e_oemid, currIdx, TRUE);
	InsertDosMember(theApp.m_dosHeaderBuf.e_oeminfo, currIdx, TRUE);
	for (int i = 0; i < 10; i++)
	{
		InsertDosMember(theApp.m_dosHeaderBuf.e_res2[i], currIdx, i == 0);
	}
	InsertDosMember(theApp.m_dosHeaderBuf.e_lfanew, currIdx, TRUE);
	RefreshListStyle();
}

void CDosHeaderDlg::RefreshListStyle()
{
	m_dosHeaderList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_dosHeaderList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_dosHeaderList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_dosHeaderList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
}


void CDosHeaderDlg::OnDblclkListDosHeader(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int curItem = m_dosHeaderList.GetSelectionMark();
	CString curFieldName = m_dosHeaderList.GetItemText(curItem, 0);
	CString curOffsetStr = m_dosHeaderList.GetItemText(curItem, 1);
	CString curSize = m_dosHeaderList.GetItemText(curItem, 2);
	CString curValue = m_dosHeaderList.GetItemText(curItem, 3);
	CChangeValDlg dlg;
	dlg.m_fieldName = curFieldName;
	dlg.m_oldVal = curValue;
	dlg.m_valOffset = theApp.GetHex(curOffsetStr.GetString());
	if (curSize == "BYTE")
	{
		dlg.m_valSize = 1;
	}
	else if(curSize == "WORD")
	{
		dlg.m_valSize = 2;
	}
	else if (curSize == "DWORD")
	{
		dlg.m_valSize = 4;
	}
	else if (curSize == "ULONGLONG")
	{
		dlg.m_valSize = 8;
	}
	dlg.DoModal();
}
