#pragma once

#include <Interfaces/ControllerBase.h>
#include <MenuController.h>
#include <Services/GameService.h>
#include <Services/UserService.h>
#include <GameModel.h>
#include <UserModel.h>
#include <Communication/CommunicationService.h>

namespace Controller
{
	class MenuController : public Interfaces::ControllerBase
	{
	public:
		MenuController(const std::shared_ptr<Communication::CommunicationService>& communicationService);
		~MenuController() = default;

		inline Model::GameModel GetGameModel() const { return _gameModel; }
		inline Model::UserModel GetUserModel() const { return _userModel; }

		void NewGame(const std::string& nickName);
		void JoinGame(const std::string& nickName, const std::string& id);
		void RecoverLastGame(const std::string& nickName);
	private:
		std::shared_ptr<Service::GameService> _gameService;
		std::shared_ptr<Service::UserService> _userService;

		Model::GameModel _gameModel;
		Model::UserModel _userModel;

		std::shared_ptr<Communication::CommunicationService> _communicationService;

		void ValidationUserAndGame(const std::string& nickName);
		void CreateConnection(const std::string& id, const bool& createGame);
		void ConnectionChannel(const std::string& id, const bool& joinGame);
	};
}

