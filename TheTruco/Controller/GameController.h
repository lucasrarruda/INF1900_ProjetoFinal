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

		void CopyGameCodetoClipboard(const std::wstring& gameCode);
		// TODO: implementar saída do jogo
		void LeaveGame();

	private:
		std::shared_ptr<Service::GameService> _gameService;
		std::shared_ptr<Service::UserService> _userService;

		std::shared_ptr<Model::GameModel> _gameModel;
		std::shared_ptr<Model::UserModel> _userModel;

		std::shared_ptr<Communication::CommunicationService> CommunicationService;
    };
}

