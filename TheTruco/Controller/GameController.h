#pragma once

#include <Interfaces/ControllerBase.h>
#include <ContentProvider.h>

namespace Controller
{
    class GameController :
        public Interfaces::ControllerBase
    {
	public:
		GameController(const std::shared_ptr<Controller::ContentProvider>& contentProvider);
		~GameController() = default;

		inline std::shared_ptr<Model::GameModel> GetGameModel() const { return _gameModel; }
		inline std::shared_ptr<Model::UserModel> GetUserModel() const { return _userModel; }
		inline std::shared_ptr<Model::PlayerModel> GetCurrentPlayerModel() const { return _currentPlayerModel; }

		void CopyGameCodetoClipboard(const std::wstring& gameCode);
		void NotifyTruco();
		void LeaveGame(const bool& otherPlayer);
		void PlayCard(const int& id);
		void PlayCoveredCard(const int& id);
		void LeaveClient();

	private:
		std::shared_ptr<Service::GameService> _gameService;
		std::shared_ptr<Service::UserService> _userService;

		std::shared_ptr<Model::GameModel> _gameModel;
		std::shared_ptr<Model::UserModel> _userModel;
		std::shared_ptr<Model::PlayerModel> _currentPlayerModel;

		std::shared_ptr<Communication::CommunicationService> CommunicationService;
    };
}

