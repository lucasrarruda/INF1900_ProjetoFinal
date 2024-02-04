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

MenuController::MenuController(const shared_ptr<CommunicationService>& communicationService): 
	_communicationService(communicationService)
{
	auto gameRepository = GameRepository();
	auto userRepository = UserRepository();

	_gameService = make_shared<GameService>(gameRepository);
	_userService = make_shared<UserService>(userRepository);

	_gameModel = make_shared<GameModel>();
	_userModel = make_shared<UserModel>();
}

void MenuController::NewGame()
{
	try
	{
		ValidationUserAndGame();

		auto playerHost = make_shared<Model::PlayerModel>();
		playerHost->SetNickName(_userModel->GetNickName());
		_gameModel = _gameService->NewGame(playerHost);
		
		_userModel->SetCurrentGameID(_gameModel->GetId());
		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		// TODO: Removi pra n�o perder a referencia do GameCode, pq esse m�todo vai apagar o id do jogo
		//CreateConnection(_gameModel->GetId(), true);

		// Nessa Etapa o usu�rio fica esperando a resposta do client (do usu�rio do JoinGame) para poder come�ar a partida
		// Precisamos mexer aqui para valida��o dessa resposta
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
		ValidationUserAndGame();

		auto player = make_shared<Model::PlayerModel>();
		player->SetNickName(_userModel->GetNickName());
		_gameModel->CopyFrom(_gameService->JoinGame(_userModel->GetId(), player));

		if (_gameModel->GetId().compare("{00000000-0000-0000-0000-000000000000}") == 0)
		{
			//Coloquei isso para exemplificar as diferentes formas de exce��es e valida��es que podem existir
			throw std::invalid_argument("O usu�rio atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
		}

		_userModel->SetCurrentGameID(_gameModel->GetId());
		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		ConnectionChannel(true);
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

void Controller::MenuController::StartJoinGame()
{
}

void MenuController::RecoverLastGame()
{
	try
	{
		ValidationUserAndGame();

		_gameModel = _gameService->RecoverLastGame(_userModel->GetCurrentGameID());

		if (_gameModel->GetId().compare("{00000000-0000-0000-0000-000000000000}") == 0)
		{
			throw std::invalid_argument("O usu�rio atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
		}

		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		auto playerHost = false;
		for (auto& player : _gameModel->GetPlayers())
		{
			if (player.second->GetNickName().compare(_userModel->GetNickName()) == 0)
			{
				playerHost = player.second->IsHostPlayer();
				break;
			}
		}

		if (playerHost)
		{
			CreateConnection(false);

			// Nessa Etapa o usu�rio fica esperando a resposta do client (do usu�rio do JoinGame) para poder come�ar a partida
			// Precisamos mexer aqui para valida��o dessa resposta
			StructMessage receivedvalue = _communicationService->ReceiveDataFromPipe();
		}
		else 
		{
			bool teste;
			ConnectionChannel(false);
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

void Controller::MenuController::Back()
{
}

void MenuController::ValidationUserAndGame()
{
	auto userModelConflicting = _userService->GetUserByNickname(_userModel);

	if (userModelConflicting->GetNickName().compare(_userModel->GetNickName()) == 0)
	{
		_userModel->CopyFrom(userModelConflicting);
	}
	else
	{
		_userModel->CopyFrom(_userService->SaveUser(_userModel));
	}

	if (_userModel->GetCurrentGameID().compare("") != 0)
	{
		throw std::exception("O usu�rio atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
	}

	if (_userModel->GetOnCurrentGame())
	{
		throw std::exception("O usu�rio est� atualmente ativo em uma partida!");
	}
}

void MenuController::CreateConnection(const bool& createGame)
{
	// Aqui deu erro ao criar a conex�o precisa ser verificado
	//string password = Utils::SplitString(_gameModel->GetId(), "{")[1];
	//password = Utils::SplitString(password, "}")[0];
	//wstring valuePassword = Serialize::ConvertStringToWString(password);
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

		throw std::exception("Ocorreu um problema na cria��o do game, tente novamente!");
	}
}

void MenuController::ConnectionChannel(const bool& joinGame)
{
	// Nessa etapa o usu�rio vai enviar uma resposta avisando para o host que logou e est� pronto para come�ar a partida
	// Precisamos mexer aqui para criar o client e enviar a resposta para o servidor

	bool getConnectChannel = false; // m�todo aqui que envia a resposta para o servidor

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
