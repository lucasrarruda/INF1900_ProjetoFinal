#pragma once

class ControllerBase
{
public:
	ControllerBase() = default;
	virtual ~ControllerBase() = 0;

	virtual void Execute() = 0;
};
