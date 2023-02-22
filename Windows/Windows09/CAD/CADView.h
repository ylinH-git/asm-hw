
// CADView.h: CCADView 类的接口
//

#pragma once
#include <functional>
#include "IShape.h"
#include "CPenSetting.h"
#include "CBrushSetting.h"

typedef std::function<void(ShapePtr)> ShapePtrFn;

class CCADView : public CView
{
private:
	CreateShapePtr m_pCurShapeCreate = nullptr;
	ShapePtr m_pCurShape = nullptr;
	CPoint m_curPoint;
	CPoint m_moveStartPoint;
	bool m_isDrawing = false;
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

private:
	void MoveHandler(const CPoint sPoint, const CPoint& ePoint, ShapePtrFn cb = nullptr);
	void ClearRedoStack();

protected: // 仅从序列化创建
	CCADView() noexcept;
	DECLARE_DYNCREATE(CCADView)

// 特性
public:
	CCADDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLine();
	afx_msg void OnMenuCircle();
	afx_msg void OnMenuRect();
	afx_msg void OnMenuStar();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSettingPen();
	void SetPenIni();
	void GetPenIni();
	afx_msg void OnSettingBrush();
	void SetBrushIni();
	void GetBrushIni();
	afx_msg void OnMenuSelect();
	afx_msg void OnMenuDelete();
	afx_msg void OnRotate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRMenuPen();
	afx_msg void OnRMenuBrush();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
};

#ifndef _DEBUG  // CADView.cpp 中的调试版本
inline CCADDoc* CCADView::GetDocument() const
   { return reinterpret_cast<CCADDoc*>(m_pDocument); }
#endif

