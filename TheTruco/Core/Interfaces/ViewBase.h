#pragma once

#include <Interfaces/ControllerBase.h>
#include <memory>

namespace Interfaces
{
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
}
