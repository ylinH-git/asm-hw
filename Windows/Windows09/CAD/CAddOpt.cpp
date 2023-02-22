#include "pch.h"
#include "CAddOpt.h"

CAddOpt::CAddOpt(CList<ShapePtr>* list, ShapePtr shape) : m_gList(list), m_shape(shape)
{
}

void CAddOpt::Undo()
{
	auto delPos = m_gList->Find(m_shape);
	m_gList->RemoveAt(delPos);
}

void CAddOpt::Redo()
{
	m_gList->AddTail(m_shape);
}