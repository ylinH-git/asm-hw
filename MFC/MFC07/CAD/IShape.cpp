#include "pch.h"
#include "IShape.h"
#include "Rotate.h"

void IShape::SetStartPoint(CPoint& sPoint)
{
	m_startPoint = sPoint;
}

void IShape::SetEndPoint(CPoint& ePoint)
{
	m_endPoint = ePoint;
}

void IShape::SetPenAttr(tagPenAttr* attr)
{
	m_penAttr = *attr;
}

void IShape::SetBrushAttr(LOGBRUSH* attr)
{
	m_brushAttr = *attr;
}

void IShape::SetState(CString stateType, BOOL value)
{
	m_stateMap[stateType] = value;
}

BOOL IShape::GetState(CString stateType)
{
	return m_stateMap[stateType];
}

void IShape::Rotate()
{
	rotate(m_startPoint, m_endPoint, PI / 2);
}

void IShape::MoveShape(CPoint begin, CPoint end)
{
	CSize sizeOff = end - begin;
	m_startPoint += sizeOff;
	m_endPoint += sizeOff;
}

CLine::CLine()
{
}

CLine::~CLine()
{
}

void CLine::DrawShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(m_penAttr.m_style, m_penAttr.m_nWidth, m_penAttr.m_color);
	CBrush brush;
	brush.CreateBrushIndirect(&m_brushAttr);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);

	pDc->MoveTo(m_startPoint);
	pDc->LineTo(m_endPoint);
	CalSelArea();

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);

	if (m_stateMap["selected"])
	{
		DrawSelectShape(pDc);
	}
}

void CLine::DrawSelectShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 1, RGB(90, 90, 90));
	CBrush brush;
	LOGBRUSH brushStyle = {
		BS_HOLLOW,
		NULL,
		NULL
	};
	brush.CreateBrushIndirect(&brushStyle);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);
	auto oldFilMode = pDc->SetPolyFillMode(WINDING);

	auto points = new CPoint[4];
	CalLineSelRect(points);


	pDc->Polygon(points, 4);
	delete[] points;

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
	pDc->SetPolyFillMode(oldFilMode);
}

void CLine::CalSelArea()
{
	// 生成选择区域
	m_selSPoint = m_startPoint;
	m_selEPoint = m_endPoint;
}

BOOL CLine::InSelectArea(CPoint point)
{
	CRgn rgn;
	auto points = new CPoint[4];
	CalLineSelRect(points);

	rgn.CreatePolygonRgn(points, 4, WINDING);
	delete[] points;
	
	return rgn.PtInRegion(point);
}

void CLine::CalLineSelRect(CPoint* points)
{
	int selSize = m_selectAreaSize + m_penAttr.m_nWidth;
	points[0].x = m_selSPoint.x;
	points[0].y = m_selSPoint.y - selSize;
	points[1].x = m_selEPoint.x + selSize;
	points[1].y = m_selEPoint.y;
	points[2].x = m_selEPoint.x;
	points[2].y = m_selEPoint.y + selSize;
	points[3].x = m_selSPoint.x - selSize;
	points[3].y = m_selSPoint.y;
}

ShapePtr ILineCreate::CreateShape()
{
	return ShapePtr((IShape*) new CLine());
}

CRectangle::CRectangle()
{
}

CRectangle::~CRectangle()
{
}

void CRectangle::DrawShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(m_penAttr.m_style, m_penAttr.m_nWidth, m_penAttr.m_color);
	CBrush brush;
	brush.CreateBrushIndirect(&m_brushAttr);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);

	pDc->Rectangle(m_startPoint.x, m_startPoint.y, m_endPoint.x, m_endPoint.y);

	CalSelArea();

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);

	if (m_stateMap["selected"])
	{
		DrawSelectShape(pDc);
	}
}

void CRectangle::CalSelArea()
{
	// 生成选择区域
	int selSize = m_selectAreaSize + m_penAttr.m_nWidth;
	m_selSPoint.x = m_startPoint.x - selSize;
	m_selSPoint.y = m_startPoint.y - selSize;
	m_selEPoint.x = m_endPoint.x + selSize;
	m_selEPoint.y = m_endPoint.y + selSize;
}

void CRectangle::DrawSelectShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 1, RGB(90, 90, 90));
	CBrush brush;
	LOGBRUSH brushStyle = {
		BS_HOLLOW,
		NULL,
		NULL
	};
	brush.CreateBrushIndirect(&brushStyle);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);

	pDc->Rectangle(m_selSPoint.x, m_selSPoint.y, m_selEPoint.x, m_selEPoint.y);

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
}

BOOL CRectangle::InSelectArea(CPoint point)
{
	CRgn rgn;
	rgn.CreateRectRgn(m_selSPoint.x, m_selSPoint.y, m_selEPoint.x, m_selEPoint.y);
	return rgn.PtInRegion(point);
}

ShapePtr IRectCreate::CreateShape()
{
	return ShapePtr((IShape*) new CRectangle());
}

CEllipse::CEllipse()
{
}

CEllipse::~CEllipse()
{
}

void CEllipse::DrawShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(m_penAttr.m_style, m_penAttr.m_nWidth, m_penAttr.m_color);
	CBrush brush;
	brush.CreateBrushIndirect(&m_brushAttr);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);
	CalSelArea();

	pDc->Ellipse(m_startPoint.x, m_startPoint.y, m_endPoint.x, m_endPoint.y);

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);	

	if (m_stateMap["selected"])
	{
		DrawSelectShape(pDc);
	}
}

void CEllipse::DrawSelectShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 1, RGB(90, 90, 90));
	CBrush brush;
	LOGBRUSH brushStyle = {
		BS_HOLLOW,
		NULL,
		NULL
	};
	brush.CreateBrushIndirect(&brushStyle);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);

	pDc->Ellipse(m_selSPoint.x, m_selSPoint.y, m_selEPoint.x, m_selEPoint.y);

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
}

void CEllipse::CalSelArea()
{
	// 生成选择区域
	int selSize = m_selectAreaSize + m_penAttr.m_nWidth;
	m_selSPoint.x = m_startPoint.x - selSize;
	m_selSPoint.y = m_startPoint.y - selSize;
	m_selEPoint.x = m_endPoint.x + selSize;
	m_selEPoint.y = m_endPoint.y + selSize;
}

BOOL CEllipse::InSelectArea(CPoint point)
{
	CRgn rgn;
	rgn.CreateEllipticRgn(m_selSPoint.x, m_selSPoint.y, m_selEPoint.x, m_selEPoint.y);
	return rgn.PtInRegion(point);
}

ShapePtr IEllipseCreate::CreateShape()
{
	return ShapePtr((IShape*) new CEllipse());
}

CStar::CStar()
{
}

CStar::~CStar()
{
}

void CStar::DrawShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(m_penAttr.m_style, m_penAttr.m_nWidth, m_penAttr.m_color);
	CBrush brush;
	brush.CreateBrushIndirect(&m_brushAttr);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);
	auto oldFilMode = pDc->SetPolyFillMode(WINDING);

	auto points = new CPoint[5];
	CalStarShape(points, m_startPoint, m_endPoint);
	CalSelArea();

	pDc->Polygon(points, 5);
	delete[] points;

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
	pDc->SetPolyFillMode(oldFilMode);

	if (m_stateMap["selected"])
	{
		DrawSelectShape(pDc);
	}
}

void CStar::CalStarShape(CPoint* points, CPoint sPoint, CPoint ePoint)
{
	const auto RX = -(sPoint.x - ePoint.x); const auto RY = -(sPoint.y - ePoint.y);
	const double PI = 3.1415926;
	points[0].x = (long)(sPoint.x + ePoint.x) / 2;
	points[0].y = sPoint.y;

	points[1].x = (long)(sPoint.x + RX * (sin(72 * PI / 180) - cos(54 * PI / 180)) / 2 / sin(72 * PI / 180));
	points[1].y = ePoint.y;

	points[2].x = ePoint.x;
	points[2].y = (long)(sPoint.y + RY * (1 - cos(72 * PI / 180)) / (1 + sin(54 * PI / 180)));

	points[3].x = sPoint.x;
	points[3].y = (long)(sPoint.y + RY * (1 - cos(72 * PI / 180)) / (1 + sin(54 * PI / 180)));

	points[4].x = (long)(sPoint.x + RX * (sin(72 * PI / 180) + cos(54 * PI / 180)) / 2 / sin(72 * PI / 180));
	points[4].y = ePoint.y;
}

void CStar::CalSelArea()
{
	// 生成选择区域
	int selSize = m_selectAreaSize + m_penAttr.m_nWidth;
	m_selSPoint.x = m_startPoint.x - selSize;
	m_selSPoint.y = m_startPoint.y - selSize;
	m_selEPoint.x = m_endPoint.x + selSize;
	m_selEPoint.y = m_endPoint.y + selSize;
}

void CStar::DrawSelectShape(CDC* pDc)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 1, RGB(90, 90, 90));
	CBrush brush;
	LOGBRUSH brushStyle = {
		BS_HOLLOW,
		NULL,
		NULL
	};
	brush.CreateBrushIndirect(&brushStyle);
	auto oldPen = pDc->SelectObject(pen);
	auto oldBrush = pDc->SelectObject(brush);
	auto oldFilMode = pDc->SetPolyFillMode(WINDING);

	auto points = new CPoint[5];
	CalStarShape(points, m_selSPoint, m_selEPoint);


	pDc->Polygon(points, 5);
	delete[] points;

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
	pDc->SetPolyFillMode(oldFilMode);
}

BOOL CStar::InSelectArea(CPoint point)
{
	CRgn rgn;
	auto points = new CPoint[5];
	CalStarShape(points, m_selSPoint, m_selEPoint);
	rgn.CreatePolygonRgn(points, 5, WINDING);
	delete[] points;
	return rgn.PtInRegion(point);
}

ShapePtr IStarCreate::CreateShape()
{
	return ShapePtr((IShape*) new CStar());
}
