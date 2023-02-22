#pragma once
#include <memory>

class IOperator;
using OptPtr = std::shared_ptr<IOperator>;
class IOperator
{
public: 
	virtual void Undo() = 0;
	virtual void Redo() = 0;
};