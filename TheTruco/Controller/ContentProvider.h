#pragma once

#include <Communication/CommunicationService.h>
#include <Services/GameService.h>
#include <Services/UserService.h>
#include <UserModel.h>
#include <GameModel.h>
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

			GameServiceInstance = std::make_shared<Service::GameService>(gameRepository);
			UserServiceInstance = std::make_shared<Service::UserService>(userRepository);

			GameModelInstance = std::make_shared<Model::GameModel>();
			UserModelInstance = std::make_shared<Model::UserModel>();
		}
		~ContentProvider() = default;

		std::shared_ptr<Communication::CommunicationService> CommunicationServiceInstance;
		std::shared_ptr<Service::GameService> GameServiceInstance;
		std::shared_ptr<Service::UserService> UserServiceInstance;
		std::shared_ptr<Model::GameModel> GameModelInstance;
		std::shared_ptr<Model::UserModel> UserModelInstance;
	};
}

