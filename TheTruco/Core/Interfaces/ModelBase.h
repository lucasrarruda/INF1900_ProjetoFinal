#pragma once

#include <Interfaces/ViewBase.h>
#include <memory>

namespace Interfaces
{
	class ModelBase
	{
	public:
		ModelBase() = default;
		virtual ~ModelBase() = default;

		void Attach(std::shared_ptr<ViewBase> observer) { _view = observer; }
		void Detach(std::shared_ptr<ViewBase> observer) { _view.reset(); }
		void Notify() { _view->Updated(); }

	protected:
		std::shared_ptr<ViewBase> _view;
	};
}
