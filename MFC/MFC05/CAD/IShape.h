#pragma once
#include <memory>

// ͼ��
class IShape
{
public:
	virtual ~IShape() = 0 {};
	virtual void DrawShape(CDC* pDc) = 0;
	void SetStartPoint(CPoint& sPoint);
	void SetEndPoint(CPoint& ePoint);

protected:
	CPoint m_startPoint;
	CPoint m_endPoint;
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