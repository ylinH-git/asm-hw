// CAttrDlg.cpp: 实现文件
//

#include "pch.h"
#include "MySpxx.h"
#include "CAttrDlg.h"
#include "afxdialogex.h"
#include <string>


// CAttrDlg 对话框

IMPLEMENT_DYNAMIC(CAttrDlg, CDialogEx)

CAttrDlg::CAttrDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ATTRBOX, pParent)
{

}

CAttrDlg::~CAttrDlg()
{
}

void CAttrDlg::SetWinBaseAttr(HWND currHwnd, CString handle, CString title, CString className)
{
	m_currHwnd = currHwnd;
	m_handle = handle;
	m_title = title;
	m_class = className;
}

void CAttrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TAB_ATTR, m_tab);
}


BEGIN_MESSAGE_MAP(CAttrDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, TAB_ATTR, &CAttrDlg::OnSelchangeTabAttr)
END_MESSAGE_MAP()


// CAttrDlg 消息处理程序


BOOL CAttrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int nIdx = 0;
	m_tab.InsertItem(nIdx++, "常规");
	m_tab.InsertItem(nIdx++, "样式");
	m_tab.InsertItem(nIdx++, "窗口");
	m_tab.InsertItem(nIdx++, "类");
	m_tab.InsertItem(nIdx++, "进程");

	InitGeneralDlg();
	InitStyleDlg();
	InitWinDlg();
	InitClassDlg();
	InitProcessDlg();

	CRect rc;
	m_tab.GetClientRect(&rc);

	rc.top += 25;
	rc.left += 5;
	rc.right -= 5;
	rc.bottom -= 5;

	m_generalDlg.MoveWindow(&rc);
	m_styleDlg.MoveWindow(&rc);
	m_winDlg.MoveWindow(&rc);
	m_classDlg.MoveWindow(&rc);
	m_prcessDlg.MoveWindow(&rc);

	m_generalDlg.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CAttrDlg::InitProcessDlg()
{
	m_prcessDlg.Create(DLG_PROCESS_ATTR, &m_tab);

	DWORD processID;
	DWORD threadID;
	threadID = ::GetWindowThreadProcessId(m_currHwnd, &processID);
	CString processStr;
	processStr.Format("%08x", processID);
	CString threadStr;
	threadStr.Format("%08x", threadID);
	m_prcessDlg.m_processId = processStr;
	m_prcessDlg.m_threadId = threadStr;

	m_prcessDlg.UpdateData(FALSE);
}

void CAttrDlg::InitClassDlg()
{
	m_classDlg.Create(DLG_CLASS_ATTR, &m_tab);
	m_classDlg.m_className = m_class;

	SetClassStyle();

	auto classByte = ::GetClassLong(m_currHwnd, GCL_CBCLSEXTRA);
	CString classByteStr;
	classByteStr.Format("%08x", classByte);
	m_classDlg.m_classByte = classByteStr;

	auto instanceHandle = ::GetWindowLong(m_currHwnd, GWL_HINSTANCE);
	CString insHandleStr;
	insHandleStr.Format("%08x", instanceHandle);
	m_classDlg.m_insHandle = insHandleStr;

	DWORD processID;
	DWORD threadID;
	threadID = ::GetWindowThreadProcessId(m_currHwnd, &processID);
	CString processStr;
	processStr.Format("%08x", processID);
	m_classDlg.m_winProcess = processStr;


	auto pMenuName = ::GetClassLong(m_currHwnd, GCL_MENUNAME);
	m_classDlg.m_menuName = (char*)pMenuName;

	auto iconHwnd = ::GetClassLong(m_currHwnd, GCL_HICON);
	CString iconHwndStr;
	iconHwndStr.Format("%08x", iconHwnd);
	m_classDlg.m_iconHandle = iconHwndStr;

	auto cursorHwnd = ::GetClassLong(m_currHwnd, GCL_HCURSOR);
	CString cursorHwndStr;
	cursorHwndStr.Format("%08x", cursorHwnd);
	m_classDlg.m_cursorHandle = cursorHwndStr;

	auto classAtom = ::GetClassLong(m_currHwnd, GCW_ATOM);
	CString atomStr;
	atomStr.Format("%d", classAtom);
	m_classDlg.m_classAtom = atomStr;

	SetClassBgPen();
	

	m_classDlg.UpdateData(FALSE);
}

void CAttrDlg::SetClassBgPen()
{
	auto classPen = ::GetClassLong(m_currHwnd, GCL_HBRBACKGROUND);
	if (classPen == NULL)
	{
		return;
	}
	CBrush bgBrush;
	bgBrush.Attach((HGDIOBJ)classPen);
	LOGBRUSH logBrush;
	bgBrush.GetLogBrush(&logBrush);
	auto burshStyle = logBrush.lbStyle;
	bgBrush.Detach();
	switch (burshStyle)
	{
	case COLOR_ACTIVEBORDER:
		m_classDlg.m_bgPen = "COLOR_ACTIVEBORDER";
		break;
	case COLOR_ACTIVECAPTION:
		m_classDlg.m_bgPen = "COLOR_ACTIVECAPTION";
		break;
	case COLOR_APPWORKSPACE:
		m_classDlg.m_bgPen = "COLOR_APPWORKSPACE";
		break;
	case COLOR_BACKGROUND:
		m_classDlg.m_bgPen = "COLOR_BACKGROUND";
		break;
	case COLOR_BTNFACE:
		m_classDlg.m_bgPen = "COLOR_BTNFACE";
		break;
	case COLOR_BTNSHADOW:
		m_classDlg.m_bgPen = "COLOR_BTNSHADOW";
		break;
	case COLOR_BTNTEXT:
		m_classDlg.m_bgPen = "COLOR_BTNTEXT";
		break;
	case COLOR_CAPTIONTEXT:
		m_classDlg.m_bgPen = "COLOR_CAPTIONTEXT";
		break;
	case COLOR_GRAYTEXT:
		m_classDlg.m_bgPen = "COLOR_GRAYTEXT";
		break;
	case COLOR_HIGHLIGHT:
		m_classDlg.m_bgPen = "COLOR_HIGHLIGHT";
		break;
	case COLOR_HIGHLIGHTTEXT:
		m_classDlg.m_bgPen = "COLOR_HIGHLIGHTTEXT";
		break;
	case COLOR_INACTIVEBORDER:
		m_classDlg.m_bgPen = "COLOR_INACTIVEBORDER";
		break;
	case COLOR_INACTIVECAPTION:
		m_classDlg.m_bgPen = "COLOR_INACTIVECAPTION";
		break;
	case COLOR_MENU:
		m_classDlg.m_bgPen = "COLOR_MENU";
		break;
	case COLOR_MENUTEXT:
		m_classDlg.m_bgPen = "COLOR_MENUTEXT";
		break;
	case COLOR_SCROLLBAR:
		m_classDlg.m_bgPen = "COLOR_SCROLLBAR";
		break;
	case COLOR_WINDOW:
		m_classDlg.m_bgPen = "COLOR_WINDOW";
		break;
	case COLOR_WINDOWFRAME:
		m_classDlg.m_bgPen = "COLOR_WINDOWFRAME";
		break;
	case COLOR_WINDOWTEXT:
		m_classDlg.m_bgPen = "COLOR_WINDOWTEXT";
		break;
	default:
		break;
	}
}

void CAttrDlg::SetClassStyle()
{
	auto classStyle = ::GetClassLong(m_currHwnd, GCL_STYLE);
	CString classStyleStr;
	classStyleStr.Format("%08x", classStyle);
	m_classDlg.m_classStyle = classStyleStr;
	if (classStyle & CS_BYTEALIGNCLIENT)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_BYTEALIGNCLIENT");
	}
	if (classStyle & CS_BYTEALIGNWINDOW)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_BYTEALIGNWINDOW");
	}
	if (classStyle & CS_CLASSDC)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_CLASSDC");
	}
	if (classStyle & CS_DBLCLKS)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_DBLCLKS");
	}
	if (classStyle & CS_DROPSHADOW)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_DROPSHADOW");
	}
	if (classStyle & CS_GLOBALCLASS)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_GLOBALCLASS");
	}
	if (classStyle & CS_HREDRAW)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_HREDRAW");
	}
	if (classStyle & CS_NOCLOSE)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_NOCLOSE");
	}
	if (classStyle & CS_OWNDC)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_OWNDC");
	}
	if (classStyle & CS_PARENTDC)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_PARENTDC");
	}
	if (classStyle & CS_SAVEBITS)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_SAVEBITS");
	}
	if (classStyle & CS_VREDRAW)
	{
		m_classDlg.m_classStyleBox.InsertString(0, "CS_VREDRAW");
	}
}

void CAttrDlg::InitWinDlg()
{
	m_winDlg.Create(DLG_WIN_ATTR, &m_tab);

	auto nextHandle = ::GetWindow(m_currHwnd, GW_HWNDNEXT);
	CString nextHandleStr;
	nextHandleStr.Format("%p", nextHandle);
	m_winDlg.m_nextHandle = nextHandleStr;
	char nextTitle[MAXBYTE];
	::GetWindowText(nextHandle, nextTitle, MAXBYTE);
	m_winDlg.m_nextTitle = nextTitle;

	auto prevHandle = ::GetWindow(m_currHwnd, GW_HWNDPREV);
	CString prevHandleStr;
	prevHandleStr.Format("%p", prevHandle);
	m_winDlg.m_prevHandle = prevHandleStr;
	char prevTitle[MAXBYTE];
	::GetWindowText(prevHandle, prevTitle, MAXBYTE);
	m_winDlg.m_prevTitle = prevTitle;

	HWND parentHandle = (HWND)::GetWindowLong(m_currHwnd, GWL_HWNDPARENT);
	CString parentHandleStr;
	parentHandleStr.Format("%p", parentHandle);
	m_winDlg.m_parentHandle = parentHandleStr;
	char parentTitle[MAXBYTE];
	::GetWindowText(parentHandle, parentTitle, MAXBYTE);
	m_winDlg.m_parentTitle = parentTitle;

	auto childHandle = ::GetWindow(m_currHwnd, GW_CHILD);
	CString childHandleStr;
	childHandleStr.Format("%p", childHandle);
	m_winDlg.m_firstHandle = childHandleStr;
	char childTitle[MAXBYTE];
	::GetWindowText(childHandle, childTitle, MAXBYTE);
	m_winDlg.m_firstTitle = childTitle;

	auto ownerHandle = ::GetWindow(m_currHwnd, GW_OWNER);
	CString ownerHandleStr;
	ownerHandleStr.Format("%p", ownerHandle);
	m_winDlg.m_ownerHandle = ownerHandleStr;
	char ownerTitle[MAXBYTE];
	::GetWindowText(ownerHandle, ownerTitle, MAXBYTE);
	m_winDlg.m_ownerTitle = ownerTitle;

	m_winDlg.UpdateData(FALSE);
}

void CAttrDlg::InitStyleDlg()
{
	m_styleDlg.Create(DLG_STYLE_ATTR, &m_tab);

	auto winStyle = ::GetWindowLong(m_currHwnd, GWL_STYLE);
	CString winStyleStr;
	winStyleStr.Format("%08x", winStyle);
	m_styleDlg.m_winStyleStr = winStyleStr;
	SetWinStyleList(winStyle);


	auto exStyle = ::GetWindowLong(m_currHwnd, GWL_EXSTYLE);
	CString exStyleStr;
	exStyleStr.Format("%08x", exStyle);
	m_styleDlg.m_exStyleStr = exStyleStr;
	SetExStyleList(exStyle);

	m_styleDlg.UpdateData(FALSE);
}

void CAttrDlg::SetExStyleList(const LONG& exStyle)
{
	if (exStyle & WS_EX_ACCEPTFILES)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_ACCEPTFILES");
	}
	if (exStyle & WS_EX_APPWINDOW)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_APPWINDOW");
	}
	if (exStyle & WS_EX_CLIENTEDGE)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_CLIENTEDGE");
	}
	if (exStyle & WS_EX_COMPOSITED)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_COMPOSITED");
	}
	if (exStyle & WS_EX_CONTEXTHELP)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_CONTEXTHELP");
	}
	if (exStyle & WS_EX_CONTROLPARENT)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_CONTROLPARENT");
	}
	if (exStyle & WS_EX_DLGMODALFRAME)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_DLGMODALFRAME");
	}
	if (exStyle & WS_EX_LAYERED)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_LAYERED");
	}
	if (exStyle & WS_EX_LAYOUTRTL)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_LAYOUTRTL");
	}
	if (exStyle & WS_EX_LEFT)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_LEFT");
	}
	if (exStyle & WS_EX_LEFTSCROLLBAR)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_LEFTSCROLLBAR");
	}
	if (exStyle & WS_EX_LTRREADING)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_LTRREADING");
	}
	if (exStyle & WS_EX_MDICHILD)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_MDICHILD");
	}
	if (exStyle & WS_EX_NOACTIVATE)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_NOACTIVATE");
	}
	if (exStyle & WS_EX_NOINHERITLAYOUT)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_NOINHERITLAYOUT");
	}
	if (exStyle & WS_EX_NOPARENTNOTIFY)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_NOPARENTNOTIFY");
	}
	if (exStyle & WS_EX_NOREDIRECTIONBITMAP)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_NOREDIRECTIONBITMAP");
	}
	if (exStyle & WS_EX_OVERLAPPEDWINDOW)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_OVERLAPPEDWINDOW");
	}
	if (exStyle & WS_EX_PALETTEWINDOW)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_PALETTEWINDOW");
	}
	if (exStyle & WS_EX_RIGHT)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_RIGHT");
	}
	if (exStyle & WS_EX_RIGHTSCROLLBAR)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_RIGHTSCROLLBAR");
	}
	if (exStyle & WS_EX_RTLREADING)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_RTLREADING");
	}
	if (exStyle & WS_EX_STATICEDGE)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_STATICEDGE");
	}
	if (exStyle & WS_EX_TOOLWINDOW)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_TOOLWINDOW");
	}
	if (exStyle & WS_EX_TOPMOST)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_TOPMOST");
	}
	if (exStyle & WS_EX_TRANSPARENT)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_TRANSPARENT");
	}
	if (exStyle & WS_EX_WINDOWEDGE)
	{
		m_styleDlg.m_exStyleList.InsertString(0, "WS_EX_WINDOWEDGE");
	}
}

void CAttrDlg::SetWinStyleList(const LONG& winStyle)
{
	if (winStyle & WS_BORDER)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_BORDER");
	}

	if (winStyle & WS_CAPTION)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_CAPTION");
	}

	if (winStyle & WS_CHILD)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_CHILD");
	}

	if (winStyle & WS_CHILDWINDOW)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_CHILDWINDOW");
	}

	if (winStyle & WS_CLIPCHILDREN)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_CLIPCHILDREN");
	}

	if (winStyle & WS_CLIPSIBLINGS)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_CLIPSIBLINGS");
	}

	if (winStyle & WS_DISABLED)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_DISABLED");
	}

	if (winStyle & WS_DLGFRAME)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_DLGFRAME");
	}

	if (winStyle & WS_GROUP)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_GROUP");
	}

	if (winStyle & WS_HSCROLL)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_HSCROLL");
	}

	if (winStyle & WS_ICONIC)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_ICONIC");
	}

	if (winStyle & WS_MAXIMIZE)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_MAXIMIZE");
	}

	if (winStyle & WS_MAXIMIZEBOX)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_MAXIMIZEBOX");
	}

	if (winStyle & WS_MINIMIZE)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_MINIMIZE");
	}

	if (winStyle & WS_MINIMIZEBOX)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_MINIMIZEBOX");
	}

	if (winStyle & WS_OVERLAPPED)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_OVERLAPPED");
	}

	if (winStyle & WS_OVERLAPPEDWINDOW)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_OVERLAPPEDWINDOW");
	}

	if (winStyle & WS_POPUP)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_POPUP");
	}

	if (winStyle & WS_POPUPWINDOW)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_POPUPWINDOW");
	}

	if (winStyle & WS_SIZEBOX)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_SIZEBOX");
	}

	if (winStyle & WS_SYSMENU)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_SYSMENU");
	}

	if (winStyle & WS_TABSTOP)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_TABSTOP");
	}


	if (winStyle & WS_THICKFRAME)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_THICKFRAME");
	}

	if (winStyle & WS_TILED)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_TILED");
	}

	if (winStyle & WS_TILEDWINDOW)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_TILEDWINDOW");
	}

	if (winStyle & WS_VISIBLE)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_VISIBLE");
	}

	if (winStyle & WS_VSCROLL)
	{
		m_styleDlg.m_winStyleList.InsertString(0, "WS_VSCROLL");
	}
}

void CAttrDlg::InitGeneralDlg()
{
	m_generalDlg.Create(DLG_GENERAL_ATTR, &m_tab);
	m_generalDlg.m_title = m_title;
	m_generalDlg.m_handle = m_handle;
	DWORD processID;
	DWORD threadID;
	threadID = ::GetWindowThreadProcessId(m_currHwnd, &processID);
	CString processStr;
	processStr.Format("%08x", processID);
	m_generalDlg.m_process = processStr;

	RECT rect;
	::GetWindowRect(m_currHwnd, &rect);
	CString rectStr;
	rectStr.Format(
		"(%d, %d)-(%d, %d), %d*%d",
		rect.left,
		rect.top,
		rect.right,
		rect.bottom,
		rect.bottom - rect.top,
		rect.right - rect.left
	);
	m_generalDlg.m_rect = rectStr;

	WINDOWPLACEMENT placement = {};
	::GetWindowPlacement(m_currHwnd, &placement);
	RECT normalRect = placement.rcNormalPosition;
	CString normalRectStr;
	normalRectStr.Format(
		"(%d, %d)-(%d, %d), %d*%d",
		normalRect.left,
		normalRect.top,
		normalRect.right,
		normalRect.bottom,
		normalRect.bottom - normalRect.top,
		normalRect.right - normalRect.left
	);
	m_generalDlg.m_normalRect = normalRectStr;

	RECT clientRect;
	::GetClientRect(m_currHwnd, &clientRect);
	CString clientRectStr;
	clientRectStr.Format(
		"(%d, %d)-(%d, %d), %d*%d",
		clientRect.left,
		clientRect.top,
		clientRect.right,
		clientRect.bottom,
		clientRect.bottom - clientRect.top,
		clientRect.right - clientRect.left
	);

	m_generalDlg.m_clientRect = clientRectStr;
	
	auto instanceHandle = ::GetWindowLong(m_currHwnd, GWL_HINSTANCE);
	CString insHandleStr;
	insHandleStr.Format("%08x", instanceHandle);
	m_generalDlg.m_insHandle = insHandleStr;

	auto menuHwnd = ::GetMenu(m_currHwnd);
	CString menuHandleStr;
	menuHandleStr.Format("%p", menuHwnd);
	m_generalDlg.m_menuHandle = menuHandleStr;

	auto userData = ::GetWindowLong(m_currHwnd, GWL_USERDATA);
	CString userDataStr;
	userDataStr.Format("%08x", userData);
	m_generalDlg.m_userData = userDataStr;

	m_generalDlg.UpdateData(FALSE);
}

void CAttrDlg::OnSelchangeTabAttr(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nIdx = m_tab.GetCurSel();
	m_generalDlg.ShowWindow(SW_HIDE);
	m_styleDlg.ShowWindow(SW_HIDE);
	m_winDlg.ShowWindow(SW_HIDE);
	m_classDlg.ShowWindow(SW_HIDE);
	m_prcessDlg.ShowWindow(SW_HIDE);

	switch (nIdx)
	{
	case 0:
		m_generalDlg.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_styleDlg.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_winDlg.ShowWindow(SW_SHOW);
		break;
	case 3:
		m_classDlg.ShowWindow(SW_SHOW);
		break;
	case 4:
		m_prcessDlg.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}
