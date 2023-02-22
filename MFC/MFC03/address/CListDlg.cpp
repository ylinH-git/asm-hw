#include "CListDlg.h"
#include "CDlgApp.h"
#include "resource.h"

CListDlg::CListDlg() : CDialogEx(DLG_SHOWCONTACT)
, m_searchKey(_T(""))
{

}


void CListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, ED_SEARCH, m_searchKey);
    DDX_Control(pDX, LT_MEMBER, m_listCtrl);
}BEGIN_MESSAGE_MAP(CListDlg, CDialogEx)
ON_BN_CLICKED(BTN_SEARCH, &CListDlg::OnBnClickedSearch)
ON_BN_CLICKED(BTN_DELETE, &CListDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


void CListDlg::OnBnClickedSearch()
{
    SearchList();
}


void CListDlg::OnBnClickedDelete()
{
    //获取列表控件选中的内容
    int nIdx = m_listCtrl.GetSelectionMark();
    int nId = m_listCtrl.GetItemData(nIdx);
    std::vector<CDlgApp::tagMember>::iterator lt;
    for (lt = theApp.g_members.begin(); lt != theApp.g_members.end(); lt++)
    {
        if (lt->m_id == nId)
        {
            break;
        }
    }
    theApp.g_members.erase(lt);
    m_listCtrl.DeleteAllItems();
    SetRows();
}


BOOL CListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO: 在此添加额外的初始化代码
    InitList();
    m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CListDlg::InitList()
{
    //添加列
    m_listCtrl.InsertColumn(0, "姓名");
    m_listCtrl.InsertColumn(1, "性别");
    m_listCtrl.InsertColumn(2, "电话");
    m_listCtrl.SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
    m_listCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
    m_listCtrl.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
    //添加内容
    SetRows();
}

void CListDlg::SetRows()
{
    //添加内容
    int nRowIdx = 0;
    for (auto lt = theApp.g_members.begin(); lt != theApp.g_members.end(); lt++)
    {
        int nColIdx = 0;
        InsertItemHandler(nRowIdx, nColIdx, lt);
        ++nRowIdx;
    }
}

void CListDlg::InsertItemHandler(int nRowIdx, int nColIdx, std::vector<CDlgApp::tagMember>::iterator lt)
{
    m_listCtrl.InsertItem(nRowIdx, lt->m_name);
    m_listCtrl.SetItemData(nRowIdx, lt->m_id);
    if (lt->m_gender == CDlgApp::eGender::MALE)
    {
        m_listCtrl.SetItemText(nRowIdx, ++nColIdx, "男");
    }
    else  if (lt->m_gender == CDlgApp::eGender::FEMALE)
    {
        m_listCtrl.SetItemText(nRowIdx, ++nColIdx, "女");
    }
    else
    {
        m_listCtrl.SetItemText(nRowIdx, ++nColIdx, "未知");
    }
    m_listCtrl.SetItemText(nRowIdx, ++nColIdx, lt->m_tel);
}


void CListDlg::SearchList()
{
    UpdateData(TRUE);
    m_listCtrl.DeleteAllItems();
    std::vector<CDlgApp::tagMember> filterMember = {};
    for (auto lt = theApp.g_members.begin(); lt != theApp.g_members.end(); lt++)
    {
        int idx = -1;
        idx = (lt->m_name).Find(m_searchKey.GetString());
        if (idx != -1)
        {
            filterMember.push_back(*lt);
        }
    }
    int nRowIdx = 0;
    for (auto lt = filterMember.begin(); lt != filterMember.end(); lt++)
    {
        int nColIdx = 0;
        InsertItemHandler(nRowIdx, nColIdx, lt);
        ++nRowIdx;
    }
}