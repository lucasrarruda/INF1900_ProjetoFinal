#pragma once

#include <Interfaces/ControllerBase.h>
#include <MenuController.h>
#include <Services/GameService.h>
#include <Services/UserService.h>
#include <GameModel.h>
#include <UserModel.h>

class MenuController : public Interfaces::ControllerBase
{
public:
	MenuController();
	~MenuController() = default;

	inline std::shared_ptr<Model::GameModel> GetGameModel() const { return _gameModel; }
	inline std::shared_ptr<Model::UserModel> GetUserModel() const { return _userModel; }

	void NewGame();
private:
	std::shared_ptr<Service::GameService> _gameService;
	std::shared_ptr<Service::UserService> _userService;

	std::shared_ptr<Model::GameModel> _gameModel;
	std::shared_ptr<Model::UserModel> _userModel;
};

