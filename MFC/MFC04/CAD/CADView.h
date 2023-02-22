
// CADView.h: CCADView 类的接口
//

#pragma once


class CCADView : public CView
{
public:
	enum class eGraphic {
		LINE,
		RECT,
		CIRCLE,
		STAR
	};
	struct tagPaintGraphic {
		CPoint m_startPoint;
		CPoint m_endPoint;
		eGraphic m_type;
	};
private:
	tagPaintGraphic m_curGraphic;
	eGraphic m_curType;
	CList<tagPaintGraphic> m_gList;
	bool m_isDrawing;

	void DrawLine(CDC* pDc, CPoint sPoint, CPoint ePoint);
	void DrawRect(CDC* pDc, CPoint sPoint, CPoint ePoint);
	void DrawEllipse(CDC* pDc, CPoint sPoint, CPoint ePoint);
	void DrawStar(CDC* pDc, CPoint sPoint, CPoint ePoint);
	void DrawGraphic(CDC* pDc, eGraphic gType, CPoint sPoint, CPoint ePoint);
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
};

#ifndef _DEBUG  // CADView.cpp 中的调试版本
inline CCADDoc* CCADView::GetDocument() const
   { return reinterpret_cast<CCADDoc*>(m_pDocument); }
#endif

