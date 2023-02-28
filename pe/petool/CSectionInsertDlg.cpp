// CSectionInsertDlg.cpp: 实现文件
//

#include "pch.h"
#include "petool.h"
#include "afxdialogex.h"
#include "CSectionInsertDlg.h"


// CSectionInsertDlg 对话框

IMPLEMENT_DYNAMIC(CSectionInsertDlg, CDialogEx)

CSectionInsertDlg::CSectionInsertDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_SECTION_INSERT, pParent)
	, m_addName(_T(""))
	, m_addMisc(_T(""))
	, m_addRva(_T(""))
	, m_addSize(_T(""))
	, m_addFa(_T(""))
	, m_addChars(_T(""))
	, m_extendName(_T(""))
	, m_extendMisc(_T(""))
	, m_extendRva(_T(""))
	, m_extendSize(_T(""))
	, m_extendFa(_T(""))
	, m_extendChars(_T(""))
{

}

CSectionInsertDlg::~CSectionInsertDlg()
{
}

void CSectionInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDT_ADD_ASM, m_addAsmEdt);
	DDX_Control(pDX, EDT_EXTEND_ASM, m_extendAsmEdt);
	DDX_Text(pDX, EDT_ADD_NAME, m_addName);
	DDX_Text(pDX, EDT_ADD_MISC, m_addMisc);
	DDX_Text(pDX, EDT_ADD_RVA, m_addRva);
	DDX_Text(pDX, EDT_ADD_SIZE, m_addSize);
	DDX_Text(pDX, EDT_ADD_FA, m_addFa);
	DDX_Text(pDX, EDT_ADD_CHARS, m_addChars);
	DDX_Text(pDX, EDT_EXTEND_NAME, m_extendName);
	DDX_Text(pDX, EDT_EXTEND_MISC, m_extendMisc);
	DDX_Text(pDX, EDT_EXTEND_RVA, m_extendRva);
	DDX_Text(pDX, EDT_EXTEND_SIZE, m_extendSize);
	DDX_Text(pDX, EDT_EXTEND_FA, m_extendFa);
	DDX_Text(pDX, EDT_EXTEND_CHARS, m_extendChars);
}


BEGIN_MESSAGE_MAP(CSectionInsertDlg, CDialogEx)
	ON_BN_CLICKED(BTN_ADD_INSERT, &CSectionInsertDlg::OnBnClickedAddInsert)
	ON_BN_CLICKED(BTN_EXTEND_INSERT, &CSectionInsertDlg::OnBnClickedExtendInsert)
END_MESSAGE_MAP()


// CSectionInsertDlg 消息处理程序


void CSectionInsertDlg::OnBnClickedAddInsert()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSectionInsertDlg::OnBnClickedExtendInsert()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CSectionInsertDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (theApp.m_sectionHeaders)
	{
		IMAGE_SECTION_HEADER lastHeader =
			theApp.m_sectionHeaders[theApp.m_sectionHeaderLen - 1];
	}
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
