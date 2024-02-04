#pragma once

#include <Interfaces/ControllerBase.h>
#include <ContentProvider.h>

namespace Controller
{
	class MenuController : public Interfaces::ControllerBase
	{
	public:
		MenuController(const std::shared_ptr<Controller::ContentProvider>& contentProvider);
		~MenuController() = default;

		inline std::shared_ptr<Model::GameModel> GetGameModel() const { return _gameModel; }
		inline std::shared_ptr<Model::UserModel> GetUserModel() const { return _userModel; }

		void NewGame();
		void JoinGame();
		void StartJoinGame(const std::string& gameCode);
		void RecoverLastGame();
		void Back();
	private:
		std::shared_ptr<Service::GameService> _gameService;
		std::shared_ptr<Service::UserService> _userService;

		std::shared_ptr<Model::GameModel> _gameModel;
		std::shared_ptr<Model::UserModel> _userModel;

		std::shared_ptr<Communication::CommunicationService> _communicationService;

		void ValidationUserAndGame();
		void CreateConnection(const bool& createGame);
		void ConnectionChannel(const bool& joinGame);
		void StartGame(const bool& messageSuccessfuly);
		void StartGameJoinGame(Communication::StructMessage response);
	};
}

