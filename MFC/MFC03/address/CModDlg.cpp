#include "CModDlg.h"
#include "CDlgApp.h"
#include "resource.h"

CModDlg::CModDlg() : CDialogEx(DLG_MODCONTACT)
, m_searchKey(_T(""))
, m_name(_T(""))
, m_tel(_T(""))
, m_gender(0)
, m_findIdx(-1)
{

}


void CModDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ED_MODSEARCH, m_searchKey);
	DDX_Text(pDX, ED_MODNAME, m_name);
	DDX_Text(pDX, ED_MODTEL, m_tel);
	DDX_Radio(pDX, RD_MODMALE, m_gender);
}BEGIN_MESSAGE_MAP(CModDlg, CDialogEx)
ON_BN_CLICKED(BTN_MODSEARCH, &CModDlg::OnBnClickedModsearch)
ON_BN_CLICKED(BTN_MODIFY, &CModDlg::OnBnClickedModify)
END_MESSAGE_MAP()


void CModDlg::OnBnClickedModsearch()
{
    UpdateData(TRUE);
    if (theApp.g_members.size() > 0)
    {
        for (auto lt = theApp.g_members.begin(); lt != theApp.g_members.end(); lt++)
        {
            m_findIdx = (lt->m_name).Find(m_searchKey.GetString());
            if (m_findIdx == -1)
            {
                AfxMessageBox("未查询到指定联络人，请重新查找");
            }
            else
            {
                m_name = lt->m_name;
                m_tel = lt->m_tel;
                m_gender = (int)lt->m_gender;
            }
        }
        UpdateData(FALSE);
    }
    else
    {
        AfxMessageBox("列表为空，请先添加联系人信息");
    }
}


void CModDlg::OnBnClickedModify()
{
    if (m_findIdx == -1)
    {
        AfxMessageBox("此操作人不存在，无法修改");
    }
    else
    {
        UpdateData(TRUE);
        CDlgApp::eGender eMemberGender = CDlgApp::eGender::MALE;
        if (m_gender == 0)
        {
            eMemberGender = CDlgApp::eGender::MALE;
        }
        if (m_gender == 1)
        {
            eMemberGender = CDlgApp::eGender::FEMALE;
        }
        if (m_gender == 2)
        {
            eMemberGender = CDlgApp::eGender::UNKNOW;
        }
        theApp.g_members[m_findIdx].m_name = m_name;
        theApp.g_members[m_findIdx].m_tel = m_tel;
        theApp.g_members[m_findIdx].m_gender = eMemberGender;
        AfxMessageBox("修改成功");
    }
}
