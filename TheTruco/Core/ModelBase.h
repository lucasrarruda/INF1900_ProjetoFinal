#pragma once

#include "ViewBase.h"
#include <memory>

class ModelBase
{
public:
	ModelBase() = default;
	virtual ~ModelBase() = default;

	virtual void Attach(std::shared_ptr<ViewBase> observer) = 0;
	virtual void Detach(std::shared_ptr<ViewBase> observer) = 0;
	virtual void Notify() = 0;

protected:
	std::shared_ptr<ViewBase> _view;
};
