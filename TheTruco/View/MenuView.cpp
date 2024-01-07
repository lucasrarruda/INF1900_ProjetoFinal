#include "pch.h"
#include "MenuView.h"
#include <MenuControllerFake.h>

using namespace std;

MenuView::MenuView(): ViewBase()
{
	_gameModel = make_shared<GameModelFake>();
	_controller = make_shared<MenuControllerFake>(_gameModel);
}

void MenuView::Updated()
{
	// Update UI with new model values
}
