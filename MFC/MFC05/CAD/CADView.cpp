
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


	auto pos = m_gList.GetHeadPosition();
	while (pos)
	{
		auto shape = m_gList.GetNext(pos);
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
	m_pCurShapeCreate = CreateShapePtr(new ILineCreate());
}


void CCADView::OnMenuCircle()
{
	m_pCurShapeCreate = CreateShapePtr(new IEllipseCreate());
}


void CCADView::OnMenuRect()
{
	m_pCurShapeCreate = CreateShapePtr(new IRectCreate());
}


void CCADView::OnMenuStar()
{
	m_pCurShapeCreate = CreateShapePtr(new IStarCreate());
}


void CCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pCurShape = m_pCurShapeCreate->CreateShape();
	if (m_pCurShapeCreate == nullptr)
	{
		return;
	}
	m_pCurShape->SetStartPoint(point);
	m_isDrawing = true;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}


void CCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pCurShape == nullptr)
	{
		return;
	}
	m_pCurShape->SetEndPoint(point);
	m_gList.AddTail(m_pCurShape);
	m_isDrawing = false;
	InvalidateRect(NULL, FALSE);
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}


void CCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isDrawing)
	{
		m_pCurShape->SetEndPoint(point);
		InvalidateRect(NULL, FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}
