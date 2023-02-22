#pragma once
#include <memory>
#include "CPenSetting.h"
#include "CBrushSetting.h"
#include <map>

// ͼ��
class IShape: public CObject
{
DECLARE_SERIAL(IShape)
public:
	IShape();
	virtual ~IShape();
	void Serialize(CArchive& archive);
	virtual void DrawShape(CDC* pDc);
	virtual void DrawSelectShape(CDC* pDc);
	void SetStartPoint(CPoint& sPoint);
	void SetEndPoint(CPoint& ePoint);
	void SetPenAttr(tagPenAttr* attr);
	tagPenAttr GetPenAttr();
	void SetBrushAttr(LOGBRUSH* attr);
	LOGBRUSH GetBrushAttr();
	void SetState(CString stateType, BOOL value);
	BOOL GetState(CString stateType);
	CPoint GetSPoint() const;
	CPoint GetEPoint() const;
	void Rotate(int degree);
	void MoveShape(CPoint begin, CPoint end);
	virtual BOOL InSelectArea(CPoint point);
	virtual void CalSelArea();
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
DECLARE_SERIAL(CLine)
public:
	CLine();
	~CLine();
	void Serialize(CArchive& archive);
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
DECLARE_SERIAL(CRectangle)
public:
	CRectangle();
	~CRectangle();
	void Serialize(CArchive& archive);
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
DECLARE_SERIAL(CEllipse)
public:
	CEllipse();
	~CEllipse();
	void Serialize(CArchive& archive);
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
DECLARE_SERIAL(CStar)
public:
	CStar();
	~CStar();
	void Serialize(CArchive& archive);
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