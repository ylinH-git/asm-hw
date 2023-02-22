#pragma once
#include "IOperator.h"
#include "IShape.h"
class CBrushOpt :
    public IOperator
{
public:
    CBrushOpt(LOGBRUSH nBrushAttr, LOGBRUSH oBrushAttr, ShapePtr shape);
    void Undo();
    void Redo();
private:
    ShapePtr m_shape;
    LOGBRUSH m_nBrushAttr;
    LOGBRUSH m_oBrushAttr;
};


