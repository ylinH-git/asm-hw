#pragma once
#include "IOperator.h"
#include "IShape.h"
class CMoveOpt :
    public IOperator
{
public:
    CMoveOpt(ShapePtr shape, CPoint sPoint, CPoint ePoint);
    void Undo();
    void Redo();
private:
    ShapePtr m_shape;
    CPoint m_sPoint;
    CPoint m_ePoint;
};

