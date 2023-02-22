#include "pch.h"
#include "CDelOpt.h"

CDelOpt::CDelOpt(CList<ShapePtr>* list, ShapePtr shape): m_gList(list), m_shape(shape)
{
}

void CDelOpt::Undo()
{
	m_gList->AddTail(m_shape);
}

void CDelOpt::Redo()
{
	auto delPos = m_gList->Find(m_shape);
	m_gList->RemoveAt(delPos);
}
