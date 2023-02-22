// MyShapeListView.cpp: 实现文件
//

#include "pch.h"
#include "CAD.h"
#include "MyShapeListView.h"
#include <typeinfo>
#include "CDelOpt.h"
#include "CRotateOpt.h"
#include "CPenOpt.h"
#include "CBrushOpt.h"


// MyShapeListView

IMPLEMENT_DYNCREATE(MyShapeListView, CListView)

MyShapeListView::MyShapeListView()
{

}

MyShapeListView::~MyShapeListView()
{
}

BEGIN_MESSAGE_MAP(MyShapeListView, CListView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(RMENU_PEN, &MyShapeListView::OnRMenuPen)
	ON_COMMAND(RMENU_BRUSH, &MyShapeListView::OnRMenuBrush)
	ON_COMMAND(MENU_DELETE, &MyShapeListView::OnMenuDelete)
	ON_COMMAND(MENU_ROTATE, &MyShapeListView::OnMenuRotate)
	ON_COMMAND(ID_EDIT_UNDO, &MyShapeListView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &MyShapeListView::OnEditRedo)
ON_NOTIFY_REFLECT(NM_CLICK, &MyShapeListView::OnNMClick)
END_MESSAGE_MAP()


// MyShapeListView 诊断

#ifdef _DEBUG
void MyShapeListView::AssertValid() const
{
	CListView::AssertValid();
}
CCADDoc* MyShapeListView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCADDoc)));
	return (CCADDoc*)m_pDocument;
}
void MyShapeListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// MyShapeListView 消息处理程序

int MyShapeListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CListCtrl& listCtr = GetListCtrl();
	listCtr.ModifyStyle(0, LVS_LIST);
	listCtr.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | listCtr.GetExtendedStyle());
	UpdateList();
	return 0;
}

void MyShapeListView::UpdateList()
{
	auto pDoc = GetDocument();
	auto pos = pDoc->m_gList.GetHeadPosition();
	CListCtrl& listCtr = GetListCtrl();
	listCtr.DeleteAllItems();
	int nRow = 0;
	while (pos)
	{
		auto shape = pDoc->m_gList.GetNext(pos);
		listCtr.InsertItem(nRow, typeid(*shape.get()).name());
		listCtr.SetItemData(nRow, (DWORD_PTR)shape.get());
		nRow++;
	}
}



void MyShapeListView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateList();
}


void MyShapeListView::OnRButtonDown(UINT nFlags, CPoint point)
{
	auto doc = GetDocument();
	auto oldState = doc->m_curState;
	doc->m_curState = CCADDoc::cursorState::SELECT;
	CMenu mn;
	mn.LoadMenu(IDR_RMENU);

	CMenu* pSubMn = mn.GetSubMenu(0);
	pSubMn->AppendMenu(MF_SEPARATOR);
	pSubMn->AppendMenu(MF_STRING, MENU_ROTATE, "旋转");
	pSubMn->AppendMenu(MF_STRING, MENU_DELETE, "删除");


	auto pos = doc->m_gList.GetTailPosition();
	int hasSelectedCount = 0;
	ShapePtr curShape = nullptr;
	while (pos)
	{
		auto shape = doc->m_gList.GetPrev(pos);
		if (shape->GetState("selected"))
		{
			curShape = shape;
			break;
		}
	}

	pSubMn->ModifyMenu(0, MF_BYPOSITION, RMENU_PEN, "画笔设置");
	pSubMn->ModifyMenu(1, MF_BYPOSITION, RMENU_BRUSH, "画刷设置");
	if (curShape != nullptr)
	{
		pSubMn->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		pSubMn->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		pSubMn->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		pSubMn->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
	}
	else
	{
		pSubMn->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
		pSubMn->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
		pSubMn->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		pSubMn->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
	}

	ClientToScreen(&point);
	pSubMn->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	doc->m_curState = oldState;
	CView::OnRButtonDown(nFlags, point);
}





void MyShapeListView::OnRMenuPen()
{
	auto doc = GetDocument();
	CPenSetting dlg;
	if (dlg.DoModal() == IDOK)
	{
		ClearRedoStack();
		auto pos = doc->m_gList.GetTailPosition();
		while (pos)
		{
			auto shape = doc->m_gList.GetPrev(pos);
			if (shape->GetState("selected"))
			{
				auto oldPenAttr = shape->GetPenAttr();
				shape->SetPenAttr(&dlg.GetPenAttr());
				doc->m_undoList.push(OptPtr(new CPenOpt(shape->GetPenAttr(), oldPenAttr, shape)));
			}
		}
	}
	InvalidateRect(NULL, FALSE);
	doc->UpdateAllViews(NULL);
}


void MyShapeListView::OnRMenuBrush()
{
	auto doc = GetDocument();
	CBrushSetting dlg;
	if (dlg.DoModal() == IDOK)
	{
		ClearRedoStack();
		auto pos = doc->m_gList.GetTailPosition();
		while (pos)
		{
			auto shape = doc->m_gList.GetPrev(pos);
			if (shape->GetState("selected"))
			{
				auto oldBrushAttr = shape->GetBrushAttr();
				shape->SetBrushAttr(&dlg.GetBrushAttr());
				doc->m_undoList.push(OptPtr(new CBrushOpt(shape->GetBrushAttr(), oldBrushAttr, shape)));
			}
		}
	}
	InvalidateRect(NULL, FALSE);
	doc->UpdateAllViews(NULL);
}


void MyShapeListView::OnMenuDelete()
{
	// TODO: 在此添加命令处理程序代码
	ClearRedoStack();
	auto doc = GetDocument();
	POSITION delPos = nullptr;
	auto pos = doc->m_gList.GetTailPosition();
	while (pos)
	{
		delPos = pos;
		auto shape = doc->m_gList.GetPrev(pos);
		if (shape->GetState("selected"))
		{
			shape->SetState("selected", FALSE);
			doc->m_undoList.push(OptPtr(new CDelOpt(&doc->m_gList, shape)));
			break;
		}
		else
		{
			delPos = nullptr;
		}
	}
	if (delPos != nullptr)
	{
		doc->m_gList.RemoveAt(delPos);
	}
	InvalidateRect(NULL, FALSE);
	doc->UpdateAllViews(NULL);
}


void MyShapeListView::OnMenuRotate()
{
	// TODO: 在此添加命令处理程序代码
	ClearRedoStack();
	auto doc = GetDocument();
	auto pos = doc->m_gList.GetTailPosition();
	while (pos)
	{
		auto shape = doc->m_gList.GetPrev(pos);
		if (shape->GetState("selected"))
		{
			doc->m_undoList.push(OptPtr(new CRotateOpt(shape, 90)));
			shape->Rotate(90);
		}
	}
	doc->UpdateAllViews(NULL);
}

void MyShapeListView::ClearRedoStack()
{
	auto pDoc = GetDocument();
	while (!pDoc->m_redoList.empty())
	{
		pDoc->m_redoList.pop();
	}
}

void MyShapeListView::OnEditUndo()
{
	auto pDoc = GetDocument();
	if (pDoc->m_undoList.empty())
	{
		return;
	}
	auto opt = pDoc->m_undoList.top();
	pDoc->m_undoList.pop();

	opt->Undo();

	pDoc->m_redoList.push(opt);
	GetDocument()->UpdateAllViews(NULL);
}


void MyShapeListView::OnEditRedo()
{
	auto pDoc = GetDocument();
	if (pDoc->m_redoList.empty())
	{
		return;
	}
	auto opt = pDoc->m_redoList.top();
	pDoc->m_redoList.pop();

	opt->Redo();

	pDoc->m_undoList.push(opt);
	GetDocument()->UpdateAllViews(NULL);
}

void MyShapeListView::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CListCtrl& listCtr = GetListCtrl();
	auto pDoc = GetDocument();
	auto nIndex = listCtr.GetSelectionMark();
	if (nIndex != -1)
	{
		auto pCurShape = (IShape*)listCtr.GetItemData(nIndex);
		auto pos = pDoc->m_gList.GetHeadPosition();
		while (pos)
		{
			auto pShape = pDoc->m_gList.GetNext(pos);
			pShape->SetState("selected", FALSE);
		}
		if (pCurShape != nullptr)
		{
			pCurShape->SetState("selected", TRUE);
		}
		auto viewPos = pDoc->GetFirstViewPosition();
		while (viewPos)
		{
			CView* pView = pDoc->GetNextView(viewPos);
			if (pView->IsKindOf(RUNTIME_CLASS(CCADView)))
			{
				pView->InvalidateRect(NULL, FALSE);
			}
		}
	}
}


BOOL MyShapeListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CListView::PreCreateWindow(cs);
}


void MyShapeListView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	CCADDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}
