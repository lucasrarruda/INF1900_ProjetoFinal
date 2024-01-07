#pragma once

#include <ControllerBase.h>
#include <GameModelFake.h>

class MenuControllerFake: public ControllerBase
{
public:
	MenuControllerFake(std::shared_ptr<GameModelFake> model);
	~MenuControllerFake() = default;

	void Execute();
private:
	std::shared_ptr<GameModelFake> _gameModel;
};

