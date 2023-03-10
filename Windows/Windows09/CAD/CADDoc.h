
// CADDoc.h: CCADDoc 类的接口
//
#include "IShape.h"
#include "IOperator.h"
#include <stack>

#pragma once


class CCADDoc : public CDocument
{
public:
	enum class cursorState {
		DRAW,
		SELECT
	};
protected: // 仅从序列化创建
	CCADDoc() noexcept;
	DECLARE_DYNCREATE(CCADDoc)

// 特性
public:
	CList<ShapePtr> m_gList;
	cursorState m_curState = cursorState::SELECT;
	std::stack<OptPtr> m_undoList;
	std::stack<OptPtr> m_redoList;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
