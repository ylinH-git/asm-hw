#include "pch.h"
#include "CPenOpt.h"

CPenOpt::CPenOpt(tagPenAttr nPenAttr, tagPenAttr oPenAttr, ShapePtr shape): m_shape(shape), m_nPenAttr(nPenAttr), m_oPenAttr(oPenAttr)
{
}

void CPenOpt::Undo()
{
	m_shape->SetPenAttr(&m_oPenAttr);
}

void CPenOpt::Redo()
{
	m_shape->SetPenAttr(&m_nPenAttr);
}
