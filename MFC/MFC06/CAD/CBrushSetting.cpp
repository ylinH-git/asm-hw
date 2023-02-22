// CBrushSetting.cpp: 实现文件
//

#include "pch.h"
#include "CAD.h"
#include "CBrushSetting.h"
#include "afxdialogex.h"


// CBrushSetting 对话框

IMPLEMENT_DYNAMIC(CBrushSetting, CDialogEx)

CBrushSetting::CBrushSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_BRUSH, pParent)
{

}

CBrushSetting::~CBrushSetting()
{
}

LOGBRUSH CBrushSetting::GetBrushAttr()
{
	return m_brushAttr;
}

void CBrushSetting::SetBrushAttr(LOGBRUSH* attr)
{
	m_brushAttr = *attr;
}

void CBrushSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, CMB_BRUSH_STYLE, m_styleCtrl);
	DDX_Control(pDX, CMB_HATCH_STYLE, m_hatchStyle);
	DDX_Control(pDX, CLB_BRUSH_COLOR, m_color);
}


BEGIN_MESSAGE_MAP(CBrushSetting, CDialogEx)
	ON_BN_CLICKED(BTN_OK, &CBrushSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CBrushSetting 消息处理程序


BOOL CBrushSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int nBrushStyleIdx = m_styleCtrl.AddString("纯色");
	m_styleCtrl.SetItemData(nBrushStyleIdx, BS_SOLID);
	nBrushStyleIdx = m_styleCtrl.AddString("空心");
	m_styleCtrl.SetItemData(nBrushStyleIdx, BS_HOLLOW);
	nBrushStyleIdx = m_styleCtrl.AddString("阴影");
	m_styleCtrl.SetItemData(nBrushStyleIdx, BS_HATCHED);

	int nHatchStyleIdx = m_hatchStyle.AddString("水平");
	m_hatchStyle.SetItemData(nHatchStyleIdx, HS_HORIZONTAL);
	nHatchStyleIdx = m_hatchStyle.AddString("垂直");
	m_hatchStyle.SetItemData(nHatchStyleIdx, HS_VERTICAL);
	nHatchStyleIdx = m_hatchStyle.AddString("左上右下");
	m_hatchStyle.SetItemData(nHatchStyleIdx, HS_FDIAGONAL);
	nHatchStyleIdx = m_hatchStyle.AddString("左下右上");
	m_hatchStyle.SetItemData(nHatchStyleIdx, HS_BDIAGONAL);
	nHatchStyleIdx = m_hatchStyle.AddString("交叉");
	m_hatchStyle.SetItemData(nHatchStyleIdx, HS_CROSS);
	nHatchStyleIdx = m_hatchStyle.AddString("斜交叉");
	m_hatchStyle.SetItemData(nHatchStyleIdx, HS_DIAGCROSS);

	m_styleCtrl.SetCurSel(m_brushAttr.lbStyle);
	m_hatchStyle.SetCurSel(m_brushAttr.lbHatch);
	m_color.SetColor(m_brushAttr.lbColor);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBrushSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGBRUSH curAttr = {
		m_styleCtrl.GetCurSel(),
		m_color.GetColor(),
		m_hatchStyle.GetCurSel(),
	};
	SetBrushAttr(&curAttr);
	EndDialog(IDOK);
}
