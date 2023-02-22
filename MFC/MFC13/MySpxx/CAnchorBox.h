#pragma once


// CAnchorBox

class CAnchorBox : public CStatic
{
	DECLARE_DYNAMIC(CAnchorBox)

public:
	CAnchorBox();
	virtual ~CAnchorBox();

protected:
	DECLARE_MESSAGE_MAP()

private:
	HWND m_hWndSave;
	BOOL m_inMove;
	void HighlightWindowBorder(HWND hWnd);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


