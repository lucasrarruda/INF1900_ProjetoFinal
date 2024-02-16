#pragma once

#include <Communication/CommunicationService.h>
#include <Services/GameService.h>
#include <Services/UserService.h>
#include <UserModel.h>
#include <GameModel.h>
#include <PlayerModel.h>
#include <memory>

namespace Controller
{
	class ContentProvider
	{
	public:
		ContentProvider()
		{
			CommunicationServiceInstance = std::make_shared<Communication::CommunicationService>();

			auto gameRepository = Repository::GameRepository();
			auto userRepository = Repository::UserRepository();

			GameModelInstance = std::make_shared<Model::GameModel>();
			UserModelInstance = std::make_shared<Model::UserModel>();
			CurrentPlayerModel = std::make_shared <Model::PlayerModel>();

			GameServiceInstance = std::make_shared<Service::GameService>(gameRepository, CommunicationServiceInstance, UserModelInstance, CurrentPlayerModel);
			UserServiceInstance = std::make_shared<Service::UserService>(userRepository);
		}
		~ContentProvider() = default;

		std::shared_ptr<Communication::CommunicationService> CommunicationServiceInstance;
		std::shared_ptr<Service::GameService> GameServiceInstance;
		std::shared_ptr<Service::UserService> UserServiceInstance;
		std::shared_ptr<Model::GameModel> GameModelInstance;
		std::shared_ptr<Model::UserModel> UserModelInstance;
		std::shared_ptr<Model::PlayerModel> CurrentPlayerModel;
	};
}

