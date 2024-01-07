#pragma once

#include "ViewBase.h"
#include <memory>

class ModelBase
{
public:
	ModelBase() = default;
	virtual ~ModelBase() = 0;

	virtual void Attach(ViewBase observer) = 0;
	virtual void Detach(ViewBase observer) = 0;
	virtual void Notify() = 0;

protected:
	std::unique_ptr<ViewBase> _view;
};
