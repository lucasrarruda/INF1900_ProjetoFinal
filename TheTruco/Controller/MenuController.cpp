#include "pch.h"
#include <MenuController.h>
#include <stdexcept>

using namespace std;
using namespace Service;
using namespace Repository;
using namespace Model;
using namespace Controller;
using namespace Helpers;
using namespace Communication;

MenuController::MenuController(const shared_ptr<CommunicationService>& communicationService)
{
	_communicationService = communicationService;

	auto gameRepository = GameRepository();
	auto userRepository = UserRepository();

	_gameService = make_shared<GameService>(gameRepository);
	_userService = make_shared<UserService>(userRepository);

	_gameModel = make_shared<GameModel>();
	_userModel = make_shared<UserModel>();
}

MenuController::MenuController(const MenuController& other): 
	_gameService(other._gameService), _userService(other._userService),
	_gameModel(other._gameModel), _userModel(other._userModel),
	_communicationService(other._communicationService)
{

}

void MenuController::NewGame()
{
	try
	{
		ValidationUserAndGame(_userModel->GetNickName());

		auto playerHost = make_shared<Model::PlayerModel>();
		playerHost->SetNickName(_userModel->GetNickName());
		_gameModel = _gameService->NewGame(playerHost);
		
		_userModel->SetCurrentGameID(_gameModel->GetId());
		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		CreateConnection(_gameModel->GetId(), true);

		// Nessa Etapa o usuário fica esperando a resposta do client (do usuário do JoinGame) para poder começar a partida
		// Precisamos mexer aqui para validação dessa resposta
		StructMessage receivedvalue = _communicationService->ReceiveDataFromPipe();
	}
	catch (const std::exception& ex)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->RemoveGame(_gameModel);
	}
}

void MenuController::JoinGame()
{
	try
	{
		ValidationUserAndGame(_userModel->GetNickName());

		auto player = make_shared<Model::PlayerModel>();
		player->SetNickName(_userModel->GetNickName());
		_gameModel = _gameService->JoinGame(_userModel->GetId(), player);

		if (_gameModel->GetId().compare("{00000000-0000-0000-0000-000000000000}") == 0)
		{
			//Coloquei isso para exemplificar as diferentes formas de exceções e validações que podem existir
			throw std::invalid_argument("O usuário atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
		}

		_userModel->SetCurrentGameID(_gameModel->GetId());
		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		ConnectionChannel(_gameModel->GetId(), true);
	}
	catch (std::invalid_argument& e)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->RemoveGame(_gameModel);
	}
	catch (std::overflow_error& ex)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->LeaveGame(_gameModel, _userModel->GetNickName());
	}
	catch (std::exception& ex)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
	}
}

void MenuController::RecoverLastGame()
{
	try
	{
		ValidationUserAndGame(_userModel->GetNickName());

		_gameModel = _gameService->RecoverLastGame(_userModel->GetCurrentGameID());

		if (_gameModel->GetId().compare("{00000000-0000-0000-0000-000000000000}") == 0)
		{
			throw std::invalid_argument("O usuário atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
		}

		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		auto playerHost = false;
		for (auto& player : _gameModel->GetPlayers())
		{
			if (player.second->GetNickName().compare(_userModel->GetNickName()) == 0)
			{
				playerHost = player.second->GetHostPlayer();
				break;
			}
		}

		if (playerHost)
		{
			CreateConnection(_gameModel->GetId(), false);

			// Nessa Etapa o usuário fica esperando a resposta do client (do usuário do JoinGame) para poder começar a partida
			// Precisamos mexer aqui para validação dessa resposta
			StructMessage receivedvalue = _communicationService->ReceiveDataFromPipe();
		}
		else 
		{
			bool teste;
			ConnectionChannel(_gameModel->GetId(), false);
		}
	}
	catch (std::invalid_argument& e)
	{
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->RemoveGame(_gameModel);
	}
	catch (const std::exception& ex)
	{
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
	}
}

void MenuController::ValidationUserAndGame(const string& nickName)
{
	_userModel->SetNickName(nickName);
	auto userModelConflicting = _userService->GetConflictingUser(_userModel);

	if (userModelConflicting->GetNickName().compare(nickName) == 0)
	{
		_userModel = userModelConflicting;
	}
	else
	{
		_userModel = _userService->SaveUser(_userModel);
	}

	if (_userModel->GetCurrentGameID().compare("") != 0)
	{
		throw std::exception("O usuário atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
	}

	if (_userModel->GetOnCurrentGame())
	{
		throw std::exception("O usuário está atualmente ativo em uma partida!");
	}
}

void MenuController::CreateConnection(const string& id, const bool& createGame)
{
	// Aqui deu erro ao criar a conexão precisa ser verificado
	//auto password = Utils::SplitString(id, "{")[1];
	//password = Utils::SplitString(password, "}")[0];
	//auto valuePassword = Serialize::ConvertStringToWString(password);
	//auto getOpenedChannel = _communicationService->OpenCommunicationChannel(valuePassword); 
	
	auto getOpenedChannel = false;

	if (!getOpenedChannel)
	{
		if (createGame)
		{
			_userModel->SetCurrentGameID("");
		}

		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);

		if (createGame)
		{
			_gameService->RemoveGame(_gameModel);
		}

		throw std::exception("Ocorreu um problema na criação do game, tente novamente!");
	}
}

void MenuController::ConnectionChannel(const string& id, const bool& joinGame)
{
	// Nessa etapa o usuário vai enviar uma resposta avisando para o host que logou e está pronto para começar a partida
	// Precisamos mexer aqui para criar o client e enviar a resposta para o servidor

	bool getConnectChannel = false; // método aqui que envia a resposta para o servidor

	if (!getConnectChannel)
	{
		if (joinGame)
		{
			_userModel->SetCurrentGameID("");
		}

		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);

		if (joinGame)
		{
			_gameService->LeaveGame(_gameModel, _userModel->GetNickName());
		}

		throw std::overflow_error("Ocorreu um problema ao entrar no game, tente novamente!");
	}
}
