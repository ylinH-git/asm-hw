#pragma once


// CColorBtn

class __declspec(dllexport) CColorBtn : public CButton
{
	DECLARE_DYNAMIC(CColorBtn)

public:
	CColorBtn();
	virtual ~CColorBtn();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnBnClicked();
private:
	COLORREF m_clr = RGB(255, 0, 0);
};


