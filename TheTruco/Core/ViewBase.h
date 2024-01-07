#pragma once

#include "ControllerBase.h"
#include <memory>

class ViewBase
{
public:
	ViewBase() = default;
	virtual ~ViewBase() = default;

	virtual void Updated() = 0;

protected:
	std::shared_ptr<ControllerBase> _controller;
};
