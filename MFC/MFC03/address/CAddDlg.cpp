#include "CAddDlg.h"
#include "CDlgApp.h"
#include "resource.h"

CAddDlg::CAddDlg() : CDialogEx(DLG_ADDCONTACT)
, m_name(_T(""))
, m_tel(_T(""))
, m_gender(0)
{

}


void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ED_NAME, m_name);
	DDX_Text(pDX, ED_TEL, m_tel);
	DDX_Radio(pDX, RD_MALE, m_gender);
}
BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_BN_CLICKED(BTN_ADDCONFIRM, &CAddDlg::OnBnClickedAddconfirm)
END_MESSAGE_MAP()

void CAddDlg::OnBnClickedAddconfirm()
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
    struct CDlgApp::tagMember member = {
        theApp.g_members.size() + 1,
        m_name,
        m_tel,
        eMemberGender
    };
    theApp.g_members.push_back(member);
    EndDialog(0);
}
