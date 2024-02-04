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
		_gameModel->CopyFrom(_gameService->NewGame(playerHost));
		
		_userModel->SetCurrentGameID(_gameModel->GetId());
		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		// TODO: Removi pra não perder a referencia do GameCode, pq esse método vai apagar o id do jogo
		//CreateConnection(_gameModel->GetId(), true);

		// Nessa Etapa o usuário fica esperando a resposta do client (do usuário do JoinGame) para poder começar a partida
		// Precisamos mexer aqui para validação dessa resposta
		StructMessage receivedValue = _communicationService->ReceiveDataFromPipe();
		StartGame(receivedValue.MessageSuccessfuly); //Resposta do player que acabou de entrar na partida
		StructMessage sendValue;
		receivedValue.MessageSuccessfuly = true;
		sendValue.Content = Serialize::ConvertGameModelToString(_gameModel); //Enviar alguma coisa aqui
		_communicationService->SendDataToPipe(sendValue); //Envia para o player a resposta que deu certo a criação do game
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
			//Coloquei isso para exemplificar as diferentes formas de exceções e validações que podem existir
			throw std::invalid_argument("O usuário atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
		}

		_userModel->SetCurrentGameID(_gameModel->GetId());
		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);
	}
	catch (std::invalid_argument& e)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->RemoveGame(_gameModel);
	}
	catch (std::exception& ex)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
	}
}

void MenuController::StartJoinGame()
{
	try
	{
		ConnectionChannel(true);

		StructMessage sendValue;
		sendValue.MessageSuccessfuly = true;
		_communicationService->SendDataToPipe(sendValue); //Envia para o player a resposta que deu certo entra no game
		StructMessage receivedValue = _communicationService->ReceiveDataFromPipe();
		StartGameJoinGame(receivedValue);
	}
	catch (std::overflow_error& ex)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->LeaveGame(_gameModel, _userModel->GetNickName());
	}
}

void MenuController::RecoverLastGame()
{
	try
	{
		ValidationUserAndGame();

		_gameModel->CopyFrom(_gameService->RecoverLastGame(_userModel->GetCurrentGameID()));

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
				playerHost = player.second->IsHostPlayer();
				break;
			}
		}

		if (playerHost)
		{
			CreateConnection(false);

			// Nessa Etapa o usuário fica esperando a resposta do client (do usuário do JoinGame) para poder começar a partida
			// Precisamos mexer aqui para validação dessa resposta
			StructMessage receivedValue = _communicationService->ReceiveDataFromPipe();
			StartGame(receivedValue.MessageSuccessfuly); //Resposta do player que acabou de entrar na partida
			StructMessage sendValue;
			receivedValue.MessageSuccessfuly = true;
			sendValue.Content = Serialize::ConvertGameModelToString(_gameModel); //Enviar alguma coisa aqui
			_communicationService->SendDataToPipe(sendValue); //Envia para o player a resposta que deu certo a criação do game
		}
		else 
		{
			ConnectionChannel(false);
			StructMessage sendValue;
			sendValue.MessageSuccessfuly = true;
			_communicationService->SendDataToPipe(sendValue); //Envia para o player a resposta que deu certo entra no game
			StructMessage receivedValue = _communicationService->ReceiveDataFromPipe();
			StartGameJoinGame(receivedValue);
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

void MenuController::Back()
{
	_userModel->SetCurrentGameID("");
	_userModel->SetOnCurrentGame(false);
	_userService->UpdateUser(_userModel);
	_gameService->LeaveGame(_gameModel, _userModel->GetNickName());
}

void MenuController::ValidationUserAndGame()
{
	auto userModelConflicting = _userService->GetUserByNickname(_userModel);

	if (_userModel->GetNickName().compare("") != 0)
	{
		throw std::exception("O usuário não pode criar um jogo com Nickname vazio!");
	}

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
		throw std::exception("O usuário atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
	}

	if (_userModel->GetOnCurrentGame())
	{
		throw std::exception("O usuário está atualmente ativo em uma partida!");
	}
}

void MenuController::CreateConnection(const bool& createGame)
{
	// Aqui deu erro ao criar a conexão precisa ser verificado
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

		throw std::exception("Ocorreu um problema na criação do game, tente novamente!");
	}
}

void MenuController::ConnectionChannel(const bool& joinGame)
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

void MenuController::StartGame(const bool& messageSuccessfuly)
{
	//Resposta positiva que o usuário se conectou no game! (Só um exemplo de resposta)
	if (messageSuccessfuly)
	{
		_gameService->StartGame(_gameModel);
		_gameService->Hand(_gameModel);
	}
	else
	{
		throw std::exception("O jogador não conseguiu se conectar na partida!");
	}
}

void MenuController::StartGameJoinGame(StructMessage response)
{
	if (response.MessageSuccessfuly == true)
	{
		_gameModel->CopyFrom(Serialize::ConvertStringToGameModel(response.Content));
	}
	else
	{
		throw std::exception("O host não conseguiu inicializar a partida!");
	}
}
