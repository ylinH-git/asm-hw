// CPenSetting.cpp: 实现文件
//

#include "pch.h"
#include "CAD.h"
#include "CPenSetting.h"
#include "afxdialogex.h"


// CPenSetting 对话框

IMPLEMENT_DYNAMIC(CPenSetting, CDialogEx)

CPenSetting::CPenSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PEN, pParent)
	, m_nWidth(0)
{

}

tagPenAttr CPenSetting::GetPenAttr()
{
	return m_penAttr;
}

void CPenSetting::SetPenAttr(tagPenAttr* attr)
{
	m_penAttr = *attr;
}

CPenSetting::~CPenSetting()
{
}

void CPenSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, CLB_PEN_COLOR, m_colorCtrl);
	DDX_Text(pDX, EDT_WIDTH, m_nWidth);
	DDX_Control(pDX, CMB_STYLE, m_styleCtrl);
}


BEGIN_MESSAGE_MAP(CPenSetting, CDialogEx)
	ON_BN_CLICKED(BTN_OK, &CPenSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CPenSetting 消息处理程序


BOOL CPenSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int nIdx = m_styleCtrl.AddString("实线");
	m_styleCtrl.SetItemData(nIdx, PS_SOLID);
	nIdx = m_styleCtrl.AddString("-------");
	m_styleCtrl.SetItemData(nIdx, PS_DASH);
	nIdx = m_styleCtrl.AddString(" .......");
	m_styleCtrl.SetItemData(nIdx, PS_DOT);
	nIdx = m_styleCtrl.AddString("_._._._");
	m_styleCtrl.SetItemData(nIdx, PS_DASHDOT);
	nIdx = m_styleCtrl.AddString(" _.._.._");
	m_styleCtrl.SetItemData(nIdx, PS_DASHDOTDOT);
	nIdx = m_styleCtrl.AddString("空");
	m_styleCtrl.SetItemData(nIdx, PS_NULL);

	m_styleCtrl.SetCurSel(m_penAttr.m_style);
	m_nWidth = m_penAttr.m_nWidth;
	m_colorCtrl.SetColor(m_penAttr.m_color);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPenSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	tagPenAttr curAttr = {
		m_styleCtrl.GetCurSel(),
		m_nWidth,
		m_colorCtrl.GetColor()
	};
	SetPenAttr(&curAttr);
	EndDialog(IDOK);
}
