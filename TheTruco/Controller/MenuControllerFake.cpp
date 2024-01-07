#include "pch.h"
#include "MenuControllerFake.h"

using namespace std;

MenuControllerFake::MenuControllerFake(std::shared_ptr<GameModelFake> model): 
	ControllerBase(), _gameModel(model)
{
	_gameModel->SetGameID("Test");
}

void MenuControllerFake::Execute()
{
	static int iterator = 0;
	_gameModel->SetGameID("Test " + to_string(iterator));
}
