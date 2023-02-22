#pragma once
#include "IOperator.h"
#include "IShape.h"
class CPenOpt :
    public IOperator
{
public:
    CPenOpt(tagPenAttr nPenAttr, tagPenAttr oPenAttr, ShapePtr shape);
    void Undo();
    void Redo();
private:
    ShapePtr m_shape;
    tagPenAttr m_nPenAttr;
    tagPenAttr m_oPenAttr;
};

