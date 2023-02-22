#pragma once


// MyShapeListView 视图
#include <afxcview.h>
class MyShapeListView : public CListView
{
	DECLARE_DYNCREATE(MyShapeListView)
protected:
	MyShapeListView();           // 动态创建所使用的受保护的构造函数
	virtual ~MyShapeListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;

	// 特性
public:
	CCADDoc* GetDocument() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateList();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRMenuPen();
	afx_msg void OnRMenuBrush();
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuRotate();
	void ClearRedoStack();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
//	afx_msg void OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* /*pDC*/);
};


