#pragma once

namespace Interfaces
{
	class ControllerBase
	{
	public:
		ControllerBase() = default;
		virtual ~ControllerBase() = default;

		virtual void Execute() = 0;
	};
}
