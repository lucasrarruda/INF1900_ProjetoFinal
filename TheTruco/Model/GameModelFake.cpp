#include "pch.h"
#include "GameModelFake.h"

void GameModelFake::Attach(std::shared_ptr<ViewBase> observer)
{
	_view = observer;
}

void GameModelFake::Detach(std::shared_ptr<ViewBase> observer)
{
	_view.reset();
}

void GameModelFake::Notify()
{
	_view->Updated();
}
