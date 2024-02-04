#pragma once

#include <Interfaces/ControllerBase.h>
#include <GameModel.h>
#include <UserModel.h>
#include <Communication/CommunicationService.h>
#include <Services/GameService.h>
#include <Services/UserService.h>

namespace Controller
{
    class GameController :
        public Interfaces::ControllerBase
    {
	public:
		GameController(const std::shared_ptr<Communication::CommunicationService>& communicationService);
		~GameController() = default;

		inline std::shared_ptr<Model::GameModel> GetGameModel() const { return _gameModel; }
		inline void SetGameModel(std::shared_ptr<Model::GameModel> model) { _gameModel = model; }

		inline std::shared_ptr<Model::UserModel> GetUserModel() const { return _userModel; }
		inline void SetUserModel(std::shared_ptr<Model::UserModel> model) { _userModel = model; }

		void CopyGameCodetoClipboard(const std::wstring& gameCode);

	private:
		std::shared_ptr<Service::GameService> _gameService;
		std::shared_ptr<Service::UserService> _userService;

		std::shared_ptr<Model::GameModel> _gameModel;
		std::shared_ptr<Model::UserModel> _userModel;

		std::shared_ptr<Communication::CommunicationService> _communicationService;
    };
}

