#pragma once

#include "ControllerBase.h"
#include <memory>

class ViewBase
{
public:
	ViewBase() = default;
	virtual ~ViewBase() = default;

	virtual void Create() = 0;
	virtual void Updated() = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;

protected:
	std::shared_ptr<ControllerBase> _controller;
};
