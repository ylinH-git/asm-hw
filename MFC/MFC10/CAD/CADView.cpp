
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
	m_curState = cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new ILineCreate());
}


void CCADView::OnMenuCircle()
{
	m_curState = cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new IEllipseCreate());
}


void CCADView::OnMenuRect()
{
	m_curState = cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new IRectCreate());
}


void CCADView::OnMenuStar()
{
	m_curState = cursorState::DRAW;
	m_pCurShapeCreate = CreateShapePtr(new IStarCreate());
}


void CCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	auto doc = GetDocument();
	switch (m_curState) 
	{
		case  cursorState::DRAW: 
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
		case  cursorState::SELECT:
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
	switch (m_curState)
	{
	case  cursorState::DRAW:
	{
		if (m_pCurShape == nullptr)
		{
			return;
		}
		ClearRedoStack();
		m_pCurShape->SetEndPoint(point);
		doc->m_gList.AddTail(m_pCurShape);
		m_undoList.push(OptPtr(new CAddOpt(&doc->m_gList, m_pCurShape)));
		m_isDrawing = false;
		ReleaseCapture();
		break;
	}
	case  cursorState::SELECT:
	default:
	{
		ClearRedoStack();
		auto moveToStack = [&](ShapePtr shape) {
			m_undoList.push(OptPtr(new CMoveOpt(shape, m_moveStartPoint, point)));
		};
		MoveHandler(m_curPoint, point, moveToStack);
		break;
	}
	}
	InvalidateRect(NULL, FALSE);
	CView::OnLButtonUp(nFlags, point);
}


void CCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isDrawing)
	{
		m_pCurShape->SetEndPoint(point);
	}

	if (m_curState == cursorState::SELECT && nFlags & MK_LBUTTON)
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
	while (!m_redoList.empty())
	{
		m_redoList.pop();
	}
}


void CCADView::OnSettingPen()
{
	CPenSetting dlg;
	dlg.SetPenAttr(&m_penAttr);
	if (dlg.DoModal() == IDOK)
	{
		m_penAttr = dlg.GetPenAttr();
	}
}


void CCADView::OnSettingBrush()
{
	CBrushSetting dlg;
	dlg.SetBrushAttr(&m_brushAttr);
	if (dlg.DoModal() == IDOK)
	{
		m_brushAttr = dlg.GetBrushAttr();
	}
}


void CCADView::OnMenuSelect()
{
	m_curState = cursorState::SELECT;
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
			m_undoList.push(OptPtr(new CDelOpt(&doc->m_gList, shape)));
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
			m_undoList.push(OptPtr(new CRotateOpt(shape, 90)));
			shape->Rotate(90);
		}
	}
}


void CCADView::OnRButtonDown(UINT nFlags, CPoint point)
{
	auto doc = GetDocument();
	auto oldState = m_curState;
	m_curState = cursorState::SELECT;
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
	m_curState = oldState;
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
				m_undoList.push(OptPtr(new CPenOpt(shape->GetPenAttr(), oldPenAttr, shape)));
			}
		}
	}
	InvalidateRect(NULL, FALSE);
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
				m_undoList.push(OptPtr(new CBrushOpt(shape->GetBrushAttr(), oldBrushAttr, shape)));
			}
		}
	}
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnEditUndo()
{
	if (m_undoList.empty())
	{
		return;
	}
	auto opt = m_undoList.top();
	m_undoList.pop();

	opt->Undo();

	m_redoList.push(opt);
	InvalidateRect(NULL, FALSE);
}


void CCADView::OnEditRedo()
{
	if (m_redoList.empty())
	{
		return;
	}
	auto opt = m_redoList.top();
	m_redoList.pop();

	opt->Redo();

	m_undoList.push(opt);
	InvalidateRect(NULL, FALSE);
}
