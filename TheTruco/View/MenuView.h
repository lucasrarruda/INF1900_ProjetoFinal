#pragma once

#include <ViewBase.h>
#include <GameModelFake.h>
#include <memory>

class MenuView: public ViewBase
{
public:
	MenuView();
	~MenuView() = default;

	void Updated();

private:
	std::shared_ptr<GameModelFake> _gameModel;
};

