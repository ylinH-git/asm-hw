#include "pch.h"
#include "CMoveOpt.h"

CMoveOpt::CMoveOpt(ShapePtr shape, CPoint sPoint, CPoint ePoint): m_shape(shape), m_sPoint(sPoint), m_ePoint(ePoint)
{
}

void CMoveOpt::Undo()
{
	m_shape->MoveShape(m_ePoint, m_sPoint);
}

void CMoveOpt::Redo()
{
	m_shape->MoveShape(m_sPoint, m_ePoint);
}
