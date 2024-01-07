#pragma once

#include "ControllerBase.h"
#include <memory>

class ViewBase
{
public:
	ViewBase() = default;
	virtual ~ViewBase() = 0;

	virtual void Updated() = 0;

protected:
	std::unique_ptr<ControllerBase> _controller;
};
