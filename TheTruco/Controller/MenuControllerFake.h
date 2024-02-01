#pragma once

#include <Interfaces/ControllerBase.h>
#include <GameModelFake.h>

class MenuControllerFake: public Interfaces::ControllerBase
{
public:
	MenuControllerFake(std::shared_ptr<GameModelFake> model);
	~MenuControllerFake() = default;

	void Execute();
private:
	std::shared_ptr<GameModelFake> _gameModel;
};

