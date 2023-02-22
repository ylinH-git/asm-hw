#include "pch.h"
#include "IShape.h"

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

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
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

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
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

	pDc->Ellipse(m_startPoint.x, m_startPoint.y, m_endPoint.x, m_endPoint.y);

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);	
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
	const auto RX = -(m_startPoint.x - m_endPoint.x); const auto RY = -(m_startPoint.y - m_endPoint.y);
	const double PI = 3.1415926;
	points[0].x = (long)(m_startPoint.x + m_endPoint.x) / 2;
	points[0].y = m_startPoint.y;

	points[1].x = (long)(m_startPoint.x + RX * (sin(72 * PI / 180) - cos(54 * PI / 180)) / 2 / sin(72 * PI / 180));
	points[1].y = m_endPoint.y;

	points[2].x = m_endPoint.x;
	points[2].y = (long)(m_startPoint.y + RY * (1 - cos(72 * PI / 180)) / (1 + sin(54 * PI / 180)));

	points[3].x = m_startPoint.x;
	points[3].y = (long)(m_startPoint.y + RY * (1 - cos(72 * PI / 180)) / (1 + sin(54 * PI / 180)));

	points[4].x = (long)(m_startPoint.x + RX * (sin(72 * PI / 180) + cos(54 * PI / 180)) / 2 / sin(72 * PI / 180));
	points[4].y = m_endPoint.y;


	pDc->Polygon(points, 5);
	delete[] points;

	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
	pDc->SetPolyFillMode(oldFilMode);
}

ShapePtr IStarCreate::CreateShape()
{
	return ShapePtr((IShape*) new CStar());
}
