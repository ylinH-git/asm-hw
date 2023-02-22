// CAnchorBox.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CAnchorBox.h"
#include "MySpxxDlg.h"

// CAnchorBox

IMPLEMENT_DYNAMIC(CAnchorBox, CStatic)

CAnchorBox::CAnchorBox()
{

}

CAnchorBox::~CAnchorBox()
{
}


BEGIN_MESSAGE_MAP(CAnchorBox, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CAnchorBox 消息处理程序




void CAnchorBox::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_inMove = TRUE;
	SetIcon(AfxGetApp()->LoadIcon(ICN_NOCROSS));
	::SetCursor(AfxGetApp()->LoadCursor(CUR_CROSS));
	SetCapture();
	CStatic::OnLButtonDown(nFlags, point);
}


void CAnchorBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    m_inMove = FALSE;
	SetIcon(AfxGetApp()->LoadIcon(ICN_CROSS));
    ReleaseCapture();
	CStatic::OnLButtonUp(nFlags, point);
}


void CAnchorBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    if (!m_inMove)
    {
        return;
    }
	ClientToScreen(&point);
	HWND hWndDst = ::WindowFromPoint(point);
    CString sHandle;
    char sClassName[MAXBYTE];
    char sTitle[MAXBYTE];
    sHandle.Format("%p", hWndDst);
    ::GetClassName(hWndDst, sClassName, MAXBYTE);
    ::GetWindowText(hWndDst, sTitle, MAXBYTE);
    pSpxxDlg->m_handle = sHandle;
    pSpxxDlg->m_currHwnd = hWndDst;
    pSpxxDlg->m_class = sClassName;
    pSpxxDlg->m_title = sTitle;
    pSpxxDlg->UpdateData(FALSE);
	if (hWndDst != m_hWndSave)
	{
		//进入了新的窗口，还原原来的窗口，并给新窗口画矩形
		if (m_hWndSave != NULL)
		{
			HighlightWindowBorder(m_hWndSave); //还原
		}

		//绘制新的
		HighlightWindowBorder(hWndDst);
		m_hWndSave = hWndDst;
	}


	CStatic::OnMouseMove(nFlags, point);
}

void CAnchorBox::HighlightWindowBorder(HWND hWnd)
{
    HDC v2; // edi
    HGDIOBJ v3; // eax
    int v4; // esi
    COLORREF v5; // eax
    HBRUSH v6; // esi
    HPEN ho; // [esp+Ch] [ebp-30h]
    HGDIOBJ v8; // [esp+10h] [ebp-2Ch]
    HGDIOBJ h; // [esp+14h] [ebp-28h]
    int v10; // [esp+18h] [ebp-24h]
    int v11; // [esp+18h] [ebp-24h]
    int w; // [esp+1Ch] [ebp-20h]
    int v13; // [esp+20h] [ebp-1Ch]
    int v14; // [esp+20h] [ebp-1Ch]
    HRGN hRgn; // [esp+24h] [ebp-18h]
    struct tagRECT Rect; // [esp+28h] [ebp-14h] BYREF

    v13 = GetSystemMetrics(5);
    v10 = GetSystemMetrics(6);
    v2 = ::GetWindowDC(hWnd);
    if (v2)
    {
        hRgn = CreateRectRgn(0, 0, 0, 0);
        ho = CreatePen(6, 3 * v13, 0);
        v8 = SelectObject(v2, ho);
        v3 = GetStockObject(5);
        h = SelectObject(v2, v3);
        SetROP2(v2, 6);
        if (::GetWindowRgn(hWnd, hRgn))
        {
            v5 = GetSysColor(6);
            v6 = CreateHatchBrush(5, v5);
            FrameRgn(v2, hRgn, v6, 3 * v13, 3 * v10);
            DeleteObject(v6);
        }
        else
        {
            w = GetSystemMetrics(32);
            v14 = GetSystemMetrics(33);
            v11 = GetSystemMetrics(0);
            v4 = GetSystemMetrics(1);
            ::GetWindowRect(hWnd, &Rect);
            if (::IsZoomed(hWnd))
                Rectangle(v2, w, v14, w + v11, v4 + v14);
            else
                Rectangle(v2, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top);
        }
        SelectObject(v2, h);
        SelectObject(v2, v8);
        DeleteObject(ho);
        DeleteObject(hRgn);
        ::ReleaseDC(hWnd, v2);
    }
}
