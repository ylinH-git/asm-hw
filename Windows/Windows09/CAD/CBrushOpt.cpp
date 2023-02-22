#include "pch.h"
#include "CBrushOpt.h"

CBrushOpt::CBrushOpt(LOGBRUSH nBrushAttr, LOGBRUSH oBrushAttr, ShapePtr shape) : m_shape(shape), m_nBrushAttr(nBrushAttr), m_oBrushAttr(oBrushAttr)
{
}

void CBrushOpt::Undo()
{
	m_shape->SetBrushAttr(&m_oBrushAttr);
}

void CBrushOpt::Redo()
{
	m_shape->SetBrushAttr(&m_nBrushAttr);
}
