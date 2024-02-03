#pragma once

#include <Interfaces/ViewBase.h>
#include <memory>
#include <list>
#include <algorithm>

namespace Interfaces
{
	class ModelBase
	{
	public:
		ModelBase() = default;
		virtual ~ModelBase() = default;

		void Attach(std::shared_ptr<ViewBase> view) 
		{ 
			_views.push_back(view); 
		}
		void Detach(std::shared_ptr<ViewBase> view) 
		{ 
			auto viewIterator = std::find(_views.cbegin(), _views.cend(), view);

			if (viewIterator != _views.end())
			{
				_views.erase(viewIterator);
			}
		}
		void Notify() 
		{
			std::for_each(_views.cbegin(), _views.cend(), [](std::shared_ptr<ViewBase> currentView)
				{
					currentView->Updated(); 
				});
		}

	protected:
		std::list<std::shared_ptr<ViewBase>> _views;
	};
}
