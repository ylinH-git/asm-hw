#pragma once
#include <memory>
#include "CPenSetting.h"
#include "CBrushSetting.h"

// ͼ��
class IShape
{
public:
	virtual ~IShape() = 0 {};
	virtual void DrawShape(CDC* pDc) = 0;
	void SetStartPoint(CPoint& sPoint);
	void SetEndPoint(CPoint& ePoint);
	void SetPenAttr(tagPenAttr* attr);
	void SetBrushAttr(LOGBRUSH* attr);
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
};


// ����Ǵ�����
class IStarCreate : public ICreateShape
{
public:
	virtual ShapePtr CreateShape();
};