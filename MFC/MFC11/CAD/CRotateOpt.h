#pragma once
#include "IOperator.h"
#include "IShape.h"
class CRotateOpt :
    public IOperator
{
public:
    CRotateOpt(ShapePtr shape, int degree);
    void Undo();
    void Redo();
private:
    ShapePtr m_shape;
    int m_degree;
};

