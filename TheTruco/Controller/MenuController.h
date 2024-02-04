#pragma once

#include <Interfaces/ControllerBase.h>
#include <MenuController.h>
#include <Services/GameService.h>
#include <Services/UserService.h>
#include <GameModel.h>00
#include <UserModel.h>
#include <Communication/CommunicationService.h>

namespace Controller
{
	class MenuController : public Interfaces::ControllerBase
	{
	public:
		MenuController(const std::shared_ptr<Communication::CommunicationService>& communicationService);
		~MenuController() = default;

		inline std::shared_ptr<Model::GameModel> GetGameModel() const { return _gameModel; }
		inline std::shared_ptr<Model::UserModel> GetUserModel() const { return _userModel; }

		void NewGame();
		void JoinGame();
		void StartJoinGame();
		void RecoverLastGame();
		void Back(bool const& newGame, bool const& joinGame);
	private:
		std::shared_ptr<Service::GameService> _gameService;
		std::shared_ptr<Service::UserService> _userService;

		std::shared_ptr<Model::GameModel> _gameModel;
		std::shared_ptr<Model::UserModel> _userModel;

		std::shared_ptr<Communication::CommunicationService> _communicationService;

		void ValidationUserAndGame();
		void CreateConnection(const std::string& id, const bool& createGame);
		void ConnectionChannel(const std::string& id, const bool& joinGame);
		void StartGame(const bool& messageSuccessfuly);
		void StartGameJoinGame(Communication::StructMessage response);
	};
}

