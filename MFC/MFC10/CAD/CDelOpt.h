#pragma once
#include "IOperator.h"
#include "IShape.h"
class CDelOpt :
    public IOperator
{
public:
    CDelOpt(CList<ShapePtr>* list, ShapePtr shape);
    void Undo();
    void Redo();
private:
    CList<ShapePtr>* m_gList;
    ShapePtr m_shape;
};

