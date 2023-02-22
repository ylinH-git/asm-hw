#include "pch.h"
#include "CRotateOpt.h"


CRotateOpt::CRotateOpt(ShapePtr shape, int degree): m_shape(shape), m_degree(degree)
{
}

void CRotateOpt::Undo()
{
	m_shape->Rotate(-m_degree);
}

void CRotateOpt::Redo()
{
	m_shape->Rotate(m_degree);
}
