#pragma once
#include <memory>
#include "CPenSetting.h"
#include "CBrushSetting.h"
#include <map>

// 图类
class IShape
{
public:
	virtual ~IShape() = 0 {};
	virtual void DrawShape(CDC* pDc) = 0;
	virtual void DrawSelectShape(CDC* pDc) = 0;
	void SetStartPoint(CPoint& sPoint);
	void SetEndPoint(CPoint& ePoint);
	void SetPenAttr(tagPenAttr* attr);
	tagPenAttr GetPenAttr();
	void SetBrushAttr(LOGBRUSH* attr);
	LOGBRUSH GetBrushAttr();
	void SetState(CString stateType, BOOL value);
	BOOL GetState(CString stateType);
	void Rotate(int degree);
	void MoveShape(CPoint begin, CPoint end);
	virtual BOOL InSelectArea(CPoint point) = 0;
	virtual void CalSelArea() = 0;
protected:
	CPoint m_startPoint;
	CPoint m_endPoint;
	tagPenAttr m_penAttr = {
		PS_SOLID,
		1,
		RGB(0, 0, 0)
	};
	LOGBRUSH  m_brushAttr = {
		BS_SOLID,
		RGB(255, 255, 255),
		HS_BDIAGONAL
	};
	std::map<CString, BOOL> m_stateMap;
	int m_selectAreaSize = 10;
	CPoint m_selSPoint;
	CPoint m_selEPoint;
};
using ShapePtr = std::shared_ptr<IShape>;

// 图创建类
class ICreateShape
{
public:
	virtual  ShapePtr CreateShape() = 0;
};
using CreateShapePtr = std::shared_ptr<ICreateShape>;


// 直线
class CLine: public IShape
{
public:
	CLine();
	~CLine();
	virtual void DrawShape(CDC* pDc);
	virtual void DrawSelectShape(CDC* pDc);
	virtual void CalSelArea();
	virtual BOOL InSelectArea(CPoint point);
	void CalLineSelRect(CPoint* points);
};

// 直线创建类
class ILineCreate: public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};

// 矩形
class CRectangle: public IShape
{
public:
	CRectangle();
	~CRectangle();
	virtual void DrawShape(CDC* pDc);
	virtual void CalSelArea();
	virtual void DrawSelectShape(CDC* pDc);
	virtual BOOL InSelectArea(CPoint point);
};

// 矩形创建类
class IRectCreate: public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};

// 椭圆
class CEllipse: public IShape
{
public:
	CEllipse();
	~CEllipse();
	virtual void DrawShape(CDC* pDc);
	virtual void DrawSelectShape(CDC* pDc);
	virtual void CalSelArea();
	virtual BOOL InSelectArea(CPoint point);
};


// 椭圆创建类
class IEllipseCreate: public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};

// 五角星
class CStar : public IShape
{
public:
	CStar();
	~CStar();
	virtual void DrawShape(CDC* pDc);
	void CalStarShape(CPoint* points, CPoint sPoint, CPoint ePoint);
	virtual void CalSelArea();
	virtual void DrawSelectShape(CDC* pDc);
	virtual BOOL InSelectArea(CPoint point);
};


// 五角星创建类
class IStarCreate : public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};