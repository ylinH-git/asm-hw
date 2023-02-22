
// CADView.cpp: CCADView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CAD.h"
#endif

#include "CADDoc.h"
#include "CADView.h"
#include "CMoveOpt.h"
#include "CDelOpt.h"
#include "CRotateOpt.h"
#include "CAddOpt.h"
#include "CPenOpt.h"
#include "CBrushOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCADView

IMPLEMENT_DYNCREATE(CCADView, CView)

BEGIN_MESSAGE_MAP(CCADView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(MENU_LINE, &CCADView::OnLine)
	ON_COMMAND(MENU_CIRCLE, &CCADView::OnMenuCircle)
	ON_COMMAND(MENU_RECT, &CCADView::OnMenuRect)
	ON_COMMAND(MENU_STAR, &CCADView::OnMenuStar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(MENU_SETTING_PEN, &CCADView::OnSettingPen)
	ON_COMMAND(MENU_SETTING_BRUSH, &CCADView::OnSettingBrush)
	ON_COMMAND(MENU_SELECT, &CCADView::OnMenuSelect)
	ON_COMMAND(MENU_DELETE, &CCADView::OnMenuDelete)
	ON_COMMAND(MENU_ROTATE, &CCADView::OnRotate)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(RMENU_PEN, &CCADView::OnRMenuPen)
	ON_COMMAND(RMENU_BRUSH, &CCADView::OnRMenuBrush)
	ON_COMMAND(ID_EDIT_UNDO, &CCADView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CCADView::OnEditRedo)
END_MESSAGE_MAP()

// CCADView 构造/析构
CCADView::CCADView() noexcept
{
	// TODO: 在此处添加构造代码
	GetBrushIni();
	GetPenIni();

}

CCADView::~CCADView()
{
}

BOOL CCADView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCADView 绘图

void CCADView::OnDraw(CDC* pDc)
{
	CCADDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rc;
	GetClientRect(&rc);

	CDC dc;
	dc.CreateCompatibleDC(pDc);



	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDc, rc.Width(), rc.Height());
	CBitmap* pOld = dc.SelectObject(&bmp);
	dc.FillSolidRect(&rc, RGB(255, 255, 255));


	auto pos = pDoc->m_gList.GetHeadPosition();
	while (pos)
	{
		auto shape = pDoc->m_gList.GetNext(pos);
		shape->DrawShape(&dc);
	}

	if (m_isDrawing && m_pCurShape != nullptr)
	{
		m_pCurShape->DrawShape(&dc);
	}



	pDc->BitBlt(0, 0, rc.Width(), rc.Height(),
		&dc, 0, 0, SRCCOPY
	);

	dc.SelectObject(pOld);

}


// CCADView 打印

BOOL CCADView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCADView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCADView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCADView 诊断

#ifdef _DEBUG
void CCADView::AssertValid() const
{
	CView::AssertValid();
}

void CCADView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCADDoc* CCADView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCADDoc)));
	return (CCADDoc*)m_pDocument;
}
#endif //_DEBUG


// CCADView 消息处理程序


void CCADView::OnLine()
{
	auto pDoc = GetDocument();
	pDoc->m_curState = CCADDoc::cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new ILineCreate());
}


void CCADView::OnMenuCircle()
{
	auto pDoc = GetDocument();
	pDoc->m_curState = CCADDoc::cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new IEllipseCreate());
}


void CCADView::OnMenuRect()
{
	auto pDoc = GetDocument();
	pDoc->m_curState = CCADDoc::cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new IRectCreate());
}


void CCADView::OnMenuStar()
{
	auto pDoc = GetDocument();
	pDoc->m_curState = CCADDoc::cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new IStarCreate());
}


void CCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	auto doc = GetDocument();
	switch (doc->m_curState)
	{
	case  CCADDoc::cursorState::DRAW:
		{
			if (m_pCurShapeCreate == nullptr)
			{
				return;
			}
			m_pCurShape = m_pCurShapeCreate->CreateShape();
			m_pCurShape->SetStartPoint(point);
			m_pCurShape->SetPenAttr(&m_penAttr);
			m_pCurShape->SetBrushAttr(&m_brushAttr);
			m_isDrawing = true;
			SetCapture();
			break;
		}
	case  CCADDoc::cursorState::SELECT:
		default:
		{
			auto pos = doc->m_gList.GetTailPosition();
			int hasSelectedCount = 0;
			while (pos)
			{
				auto shape = doc->m_gList.GetPrev(pos);
				if (shape->InSelectArea(point) && hasSelectedCount < 1)
				{
					shape->SetState("selected", TRUE);
					hasSelectedCount++;
				}
				else
				{
					shape->SetState("selected", FALSE);
				}
			}
			m_curPoint = point;
			m_moveStartPoint = point;
			break;
		}
	}
	
	CView::OnLButtonDown(nFlags, point);
}


void CCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	auto doc = GetDocument();
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (doc->m_curState)
	{
	case  CCADDoc::cursorState::DRAW:
	{
		if (m_pCurShape == nullptr)
		{
			return;
		}
		ClearRedoStack();
		if (m_pCurShape->GetSPoint() != point) {
			m_pCurShape->SetEndPoint(point);
			doc->m_gList.AddTail(m_pCurShape);
			doc->m_undoList.push(OptPtr(new CAddOpt(&doc->m_gList, m_pCurShape)));
		}
		m_isDrawing = false;
		ReleaseCapture();
		break;
	}
	case  CCADDoc::cursorState::SELECT:
	default:
	{
		ClearRedoStack();
		auto moveToStack = [&](ShapePtr shape) {
			doc->m_undoList.push(OptPtr(new CMoveOpt(shape, m_moveStartPoint, point)));
		};
		MoveHandler(m_curPoint, point, moveToStack);
		break;
	}
	}
	InvalidateRect(NULL, FALSE);
	doc->UpdateAllViews(NULL);
	CView::OnLButtonUp(nFlags, point);
}


void CCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	auto pDoc = GetDocument();
	if (m_isDrawing)
	{
		m_pCurShape->SetEndPoint(point);
	}

	if (pDoc->m_curState == CCADDoc::cursorState::SELECT && nFlags & MK_LBUTTON)
	{
		MoveHandler(m_curPoint, point);
		m_curPoint = point;
	}

	InvalidateRect(NULL, FALSE);
	CView::OnMouseMove(nFlags, point);
}

void CCADView::MoveHandler(const CPoint sPoint, const CPoint& ePoint, ShapePtrFn cb)
{
	auto doc = GetDocument();
	auto pos = doc->m_gList.GetTailPosition();
	int hasSelectedCount = 0;
	while (pos)
	{
		auto shape = doc->m_gList.GetPrev(pos);
		if (shape->GetState("selected"))
		{
			shape->MoveShape(sPoint, ePoint);
			if (cb != nullptr)
			{
				cb(shape);
			}
		}
	}
}

void CCADView::ClearRedoStack()
{
	auto pDoc = GetDocument();
	while (!pDoc->m_redoList.empty())
	{
		pDoc->m_redoList.pop();
	}
}


void CCADView::OnSettingPen()
{
	CPenSetting dlg;
	GetPenIni();
	dlg.SetPenAttr(&m_penAttr);
	if (dlg.DoModal() == IDOK)
	{
		m_penAttr = dlg.GetPenAttr();
		SetPenIni();
	}
}

void CCADView::SetPenIni()
{
	CString penBs;
	penBs.Format("%d", m_penAttr.m_style);
	BOOL bRet = WritePrivateProfileString(
		"pen",
		"penBS",
		penBs.GetString(),
		"./config.ini"
	);
	CString penWidth;
	penWidth.Format("%d", m_penAttr.m_nWidth);
	bRet = WritePrivateProfileString(
		"pen",
		"penWidth",
		penWidth.GetString(),
		"./config.ini"
	);
	CString penColor;
	penColor.Format("%08x", m_penAttr.m_color);
	bRet = WritePrivateProfileString(
		"pen",
		"penColor",
		penColor.GetString(),
		"./config.ini"
	);
}

void CCADView::GetPenIni()
{
	//PS_SOLID,
	//1,
	//RGB(0, 0, 0)
	char szPenBs[MAXBYTE] = { 0 };
	DWORD bsLen = GetPrivateProfileString("pen",
		"penBS",
		NULL,
		szPenBs,
		sizeof(szPenBs),
		"./config.ini"
	);
	if (bsLen)
	{
		m_penAttr.m_style = atoi(szPenBs);
	}
	char szPenWidth[MAXBYTE] = { 0 };
	DWORD widthLen = GetPrivateProfileString("pen",
		"penWidth",
		NULL,
		szPenWidth,
		sizeof(szPenWidth),
		"./config.ini"
	);
	if (widthLen)
	{
		m_penAttr.m_nWidth = atoi(szPenWidth);
	}
	char szPenColor[MAXBYTE] = { 0 };
	DWORD colorLen = GetPrivateProfileString("pen",
		"penColor",
		NULL,
		szPenColor,
		sizeof(szPenColor),
		"./config.ini"
	);
	if (colorLen)
	{
		DWORD lbColor;
		sscanf_s(szPenColor, "%08x", &lbColor);
		m_penAttr.m_color = lbColor;
	}
}


void CCADView::OnSettingBrush()
{
	CBrushSetting dlg;
	GetBrushIni();
	dlg.SetBrushAttr(&m_brushAttr);
	if (dlg.DoModal() == IDOK)
	{
		m_brushAttr = dlg.GetBrushAttr();
		SetBrushIni();
	}
}

void CCADView::SetBrushIni()
{
	CString brushBs;
	brushBs.Format("%d", m_brushAttr.lbStyle);
	BOOL bRet = WritePrivateProfileString(
		"brush",
		"brushBS",
		brushBs.GetString(),
		"./config.ini"
	);
	CString brushHs;
	brushHs.Format("%d", m_brushAttr.lbHatch);
	bRet = WritePrivateProfileString(
		"brush",
		"brushHS",
		brushHs.GetString(),
		"./config.ini"
	);
	CString brushColor;
	brushColor.Format("%08x", m_brushAttr.lbColor);
	bRet = WritePrivateProfileString(
		"brush",
		"brushColor",
		brushColor.GetString(),
		"./config.ini"
	);
}

void CCADView::GetBrushIni()
{
	//BS_SOLID,
	//RGB(255, 255, 255),
	//HS_BDIAGONAL
	char szBrushBs[MAXBYTE] = { 0 };
	DWORD bsLen = GetPrivateProfileString("brush",
		"brushBS",
		NULL,
		szBrushBs,
		sizeof(szBrushBs),
		"./config.ini"
	);
	if (bsLen)
	{
		m_brushAttr.lbStyle = atoi(szBrushBs);
	}
	char szBrushHs[MAXBYTE] = { 0 };
	DWORD hsLen = GetPrivateProfileString("brush",
		"brushHS",
		NULL,
		szBrushHs,
		sizeof(szBrushHs),
		"./config.ini"
	);
	if (hsLen)
	{
		m_brushAttr.lbHatch = atoi(szBrushHs);
	}
	char szBrushColor[MAXBYTE] = { 0 };
	DWORD colorLen = GetPrivateProfileString("brush",
		"brushColor",
		NULL,
		szBrushColor,
		sizeof(szBrushColor),
		"./config.ini"
	);
	if (colorLen)
	{
		DWORD lbColor;
		sscanf_s(szBrushColor, "%08x", &lbColor);
		m_brushAttr.lbColor = lbColor;
	}
}


void CCADView::OnMenuSelect()
{
	auto pDoc = GetDocument();
	pDoc->m_curState = CCADDoc::cursorState::SELECT;
}


void CCADView::OnMenuDelete()
{
	// TODO: 在此添加命令处理程序代码
	ClearRedoStack();
	auto doc = GetDocument();
	POSITION delPos = nullptr;
	auto pos = doc->m_gList.GetTailPosition();
	while (pos)
	{
		delPos = pos;
		auto shape = doc->m_gList.GetPrev(pos);
		if (shape->GetState("selected"))
		{
			shape->SetState("selected", FALSE);
			doc->m_undoList.push(OptPtr(new CDelOpt(&doc->m_gList, shape)));
			break;
		}
		else
		{
			delPos = nullptr;
		}
	}
	if (delPos != nullptr)
	{
		doc->m_gList.RemoveAt(delPos);
	}
	doc->UpdateAllViews(NULL);
}


void CCADView::OnRotate()
{
	// TODO: 在此添加命令处理程序代码
	ClearRedoStack();
	auto doc = GetDocument();
	auto pos = doc->m_gList.GetTailPosition();
	while (pos)
	{
		auto shape = doc->m_gList.GetPrev(pos);
		if (shape->GetState("selected"))
		{
			doc->m_undoList.push(OptPtr(new CRotateOpt(shape, 90)));
			shape->Rotate(90);
		}
	}
	doc->UpdateAllViews(NULL);
}


void CCADView::OnRButtonDown(UINT nFlags, CPoint point)
{
	auto doc = GetDocument();
	auto oldState = doc->m_curState;
	doc->m_curState = CCADDoc::cursorState::SELECT;
	CMenu mn;
	mn.LoadMenu(IDR_RMENU);

	CMenu* pSubMn = mn.GetSubMenu(0);
	pSubMn->AppendMenu(MF_SEPARATOR);
	pSubMn->AppendMenu(MF_STRING, MENU_ROTATE, "旋转");
	pSubMn->AppendMenu(MF_STRING, MENU_DELETE, "删除");

	auto pos = doc->m_gList.GetTailPosition();
	int hasSelectedCount = 0;
	ShapePtr curShape = nullptr;
	while (pos)
	{
		auto shape = doc->m_gList.GetPrev(pos);
		if (shape->InSelectArea(point) && hasSelectedCount < 1)
		{
			shape->SetState("selected", TRUE);
			hasSelectedCount++;
			curShape = shape;
		}
		else
		{
			shape->SetState("selected", FALSE);
		}
	}
	if (curShape != nullptr)
	{
		pSubMn->ModifyMenu(0, MF_BYPOSITION, RMENU_PEN, "画笔设置");
		pSubMn->ModifyMenu(1, MF_BYPOSITION, RMENU_BRUSH, "画刷设置");
		pSubMn->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		pSubMn->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		pSubMn->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
	}
	else
	{
		pSubMn->ModifyMenu(0, MF_BYPOSITION, MENU_SETTING_PEN, "画笔设置");
		pSubMn->ModifyMenu(1, MF_BYPOSITION, MENU_SETTING_BRUSH, "画刷设置");
		pSubMn->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		pSubMn->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
	}

	ClientToScreen(&point);
	pSubMn->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	InvalidateRect(NULL, FALSE);
	doc->m_curState = oldState;
	CView::OnRButtonDown(nFlags, point);
}


void CCADView::OnRMenuPen()
{
	auto doc = GetDocument();
	CPenSetting dlg;
	if (dlg.DoModal() == IDOK)
	{
		ClearRedoStack();
		auto pos = doc->m_gList.GetTailPosition();
		while (pos)
		{
			auto shape = doc->m_gList.GetPrev(pos);
			if (shape->GetState("selected"))
			{
				auto oldPenAttr = shape->GetPenAttr();
				shape->SetPenAttr(&dlg.GetPenAttr());
				doc->m_undoList.push(OptPtr(new CPenOpt(shape->GetPenAttr(), oldPenAttr, shape)));
			}
		}
	}
	InvalidateRect(NULL, FALSE);
	doc->UpdateAllViews(NULL);
}


void CCADView::OnRMenuBrush()
{
	auto doc = GetDocument();
	CBrushSetting dlg;
	if (dlg.DoModal() == IDOK)
	{
		ClearRedoStack();
		auto pos = doc->m_gList.GetTailPosition();
		while (pos)
		{
			auto shape = doc->m_gList.GetPrev(pos);
			if (shape->GetState("selected"))
			{
				auto oldBrushAttr = shape->GetBrushAttr();
				shape->SetBrushAttr(&dlg.GetBrushAttr());
				doc->m_undoList.push(OptPtr(new CBrushOpt(shape->GetBrushAttr(), oldBrushAttr, shape)));
			}
		}
	}
	InvalidateRect(NULL, FALSE);
	doc->UpdateAllViews(NULL);
}


void CCADView::OnEditUndo()
{
	auto pDoc = GetDocument();
	if (pDoc->m_undoList.empty())
	{
		return;
	}
	auto opt = pDoc->m_undoList.top();
	pDoc->m_undoList.pop();

	opt->Undo();

	pDoc->m_redoList.push(opt);
	InvalidateRect(NULL, FALSE);
	GetDocument()->UpdateAllViews(NULL);
}


void CCADView::OnEditRedo()
{
	auto pDoc = GetDocument();
	if (pDoc->m_redoList.empty())
	{
		return;
	}
	auto opt = pDoc->m_redoList.top();
	pDoc->m_redoList.pop();

	opt->Redo();

	pDoc->m_undoList.push(opt);
	InvalidateRect(NULL, FALSE);
	GetDocument()->UpdateAllViews(NULL);
}
