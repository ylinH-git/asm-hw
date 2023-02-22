#pragma once
#include <memory>
#include "CPenSetting.h"
#include "CBrushSetting.h"
#include <map>

// ͼ��
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

// ͼ������
class ICreateShape
{
public:
	virtual  ShapePtr CreateShape() = 0;
};
using CreateShapePtr = std::shared_ptr<ICreateShape>;


// ֱ��
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

// ֱ�ߴ�����
class ILineCreate: public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};

// ����
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

// ���δ�����
class IRectCreate: public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};

// ��Բ
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


// ��Բ������
class IEllipseCreate: public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};

// �����
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


// ����Ǵ�����
class IStarCreate : public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};