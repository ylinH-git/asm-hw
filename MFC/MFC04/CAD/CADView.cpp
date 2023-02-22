
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
END_MESSAGE_MAP()

// CCADView 构造/析构

void CCADView::DrawLine(CDC* pDc, CPoint sPoint, CPoint ePoint)
{
	pDc->MoveTo(sPoint);
	pDc->LineTo(ePoint);
}

void CCADView::DrawGraphic(CDC* pDc, eGraphic gType, CPoint sPoint, CPoint ePoint)
{
	switch (gType)
	{
	case eGraphic::LINE:
	{
		DrawLine(pDc, sPoint, ePoint);
		break;
	}
	case eGraphic::RECT:
	{
		DrawRect(pDc, sPoint, ePoint);
		break;
	}
	case eGraphic::CIRCLE:
	{
		DrawEllipse(pDc, sPoint, ePoint);
		break;
	}
	case eGraphic::STAR:
	{
		DrawStar(pDc, sPoint, ePoint);
		break;
	}
	default: break;
	}
}

void CCADView::DrawRect(CDC* pDc, CPoint sPoint, CPoint ePoint)
{
	pDc->Rectangle(sPoint.x, sPoint.y, ePoint.x, ePoint.y);
}

void CCADView::DrawEllipse(CDC* pDc, CPoint sPoint, CPoint ePoint)
{
	pDc->Ellipse(sPoint.x, sPoint.y, ePoint.x, ePoint.y);
}

void CCADView::DrawStar(CDC* pDc, CPoint sPoint, CPoint ePoint)
{
	pDc->SetPolyFillMode(WINDING);
	auto points = new CPoint[5];
	const auto RX = -(sPoint.x - ePoint.x); const auto RY = -(sPoint.y - ePoint.y);
	const double PI = 3.1415926;
	points[0].x = (sPoint.x + ePoint.x) / 2;
	points[0].y = sPoint.y;

	points[1].x = (sPoint.x + RX * (sin(72 * PI / 180) - cos(54 * PI / 180)) / 2 / sin(72 * PI / 180));
	points[1].y = ePoint.y;

	points[2].x = ePoint.x;
	points[2].y = sPoint.y + RY * (1 - cos(72 * PI / 180)) / (1 + sin(54 * PI / 180));

	points[3].x = sPoint.x;
	points[3].y = sPoint.y + RY * (1 - cos(72 * PI / 180)) / (1 + sin(54 * PI / 180));

	points[4].x = (sPoint.x + RX * (sin(72 * PI / 180) + cos(54 * PI / 180)) / 2 / sin(72 * PI / 180));
	points[4].y = ePoint.y;

	
	pDc->Polygon(points, 5);
	pDc->SetPolyFillMode(ALTERNATE);
	delete[] points;
}

CCADView::CCADView() noexcept : m_curType(eGraphic::LINE), m_isDrawing(false)
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


	auto pos = m_gList.GetHeadPosition();
	while (pos)
	{
		auto& graphic = m_gList.GetNext(pos);
		DrawGraphic(&dc, graphic.m_type, graphic.m_startPoint, graphic.m_endPoint);
	}

	if (m_isDrawing)
	{
		DrawGraphic(&dc, m_curType, m_curGraphic.m_startPoint, m_curGraphic.m_endPoint);
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
	// TODO: 在此添加命令处理程序代码
	m_curType = eGraphic::LINE;
}


void CCADView::OnMenuCircle()
{
	// TODO: 在此添加命令处理程序代码
	m_curType = eGraphic::CIRCLE;
}


void CCADView::OnMenuRect()
{
	// TODO: 在此添加命令处理程序代码
	m_curType = eGraphic::RECT;
}


void CCADView::OnMenuStar()
{
	// TODO: 在此添加命令处理程序代码
	m_curType = eGraphic::STAR;
}


void CCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_curGraphic.m_type = m_curType;
	m_curGraphic.m_startPoint = point;
	m_isDrawing = true;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}


void CCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_curGraphic.m_endPoint = point;
	m_gList.AddTail(m_curGraphic);
	m_isDrawing = false;
	InvalidateRect(NULL, FALSE);
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}


void CCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_LBUTTON)
	{
		m_curGraphic.m_endPoint = point;
		InvalidateRect(NULL, FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}
