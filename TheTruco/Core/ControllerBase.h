#pragma once

class ControllerBase
{
public:
	ControllerBase() = default;
	virtual ~ControllerBase() = default;

	virtual void Execute() = 0;
};
