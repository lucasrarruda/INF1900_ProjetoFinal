#include "pch.h"
#include <MenuController.h>

using namespace std;
using namespace Service;
using namespace Repository;

MenuController::MenuController()
{
	auto gameRepository = GameRepository();
	auto userRepository = UserRepository();

	_gameService = make_shared<GameService>(gameRepository);
	_userService = make_shared<UserService>(userRepository);

	_gameModel = make_shared<Model::GameModel>();
	_userModel = make_shared<Model::UserModel>();
}

void MenuController::NewGame()
{
}
