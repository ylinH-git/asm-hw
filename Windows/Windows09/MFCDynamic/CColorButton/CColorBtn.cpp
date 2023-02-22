// CColorBtn.cpp: 实现文件
//

#include "pch.h"
#include "CColorBtn.h"


// CColorBtn

IMPLEMENT_DYNAMIC(CColorBtn, CButton)

CColorBtn::CColorBtn()
{

}

CColorBtn::~CColorBtn()
{
}


BEGIN_MESSAGE_MAP(CColorBtn, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CColorBtn::OnBnClicked)
END_MESSAGE_MAP()



// CColorBtn 消息处理程序


void CColorBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CBrush brush;
	brush.CreateSolidBrush(m_clr);
	CBrush* pOld = dc.SelectObject(&brush);

	dc.Rectangle(&lpDrawItemStruct->rcItem);

	dc.Detach();
}


void CColorBtn::OnBnClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg(m_clr);
	dlg.DoModal();
	m_clr = dlg.GetColor();
	InvalidateRect(NULL, FALSE);
}
