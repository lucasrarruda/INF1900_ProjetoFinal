#include "pch.h"
#include <MenuController.h>
#include <stdexcept>
#include <Exceptions/MatchInProgress.h>
#include <Exceptions/GameInvalid.h>
#include <future>

using namespace std;
using namespace Service;
using namespace Repository;
using namespace Model;
using namespace Controller;
using namespace Helpers;
using namespace Communication;
using namespace Exceptions;

MenuController::MenuController(const std::shared_ptr<Controller::ContentProvider>& contentProvider):
	_communicationService(contentProvider->CommunicationServiceInstance)
{
	_gameService = contentProvider->GameServiceInstance;
	_userService = contentProvider->UserServiceInstance;

	_gameModel = contentProvider->GameModelInstance;
	_userModel = contentProvider->UserModelInstance;
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

		CreateConnection(true);
	}
	catch (const GameInvalid& ex)
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
	}
	catch (const GameInvalid& ex)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
	}
}

void MenuController::StartJoinGame(const std::string& gameCode)
{
	try
	{
		auto player = make_shared<Model::PlayerModel>();
		player->SetNickName(_userModel->GetNickName());
		_gameModel->CopyFrom(_gameService->JoinGame(gameCode, player));

		if (_gameModel->GetId().compare("{00000000-0000-0000-0000-000000000000}") == 0)
		{
			throw MatchInProgress("O usu�rio atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
		}

		_userModel->SetCurrentGameID(_gameModel->GetId());
		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		ConnectionChannel(true);
	}
	catch (const MatchInProgress& e)
	{
		_userModel->SetCurrentGameID("");
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->RemoveGame(_gameModel);
	}
	catch (const GameInvalid& ex)
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
			throw MatchInProgress("O usu�rio atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
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
		}
		else 
		{
			ConnectionChannel(false);
		}
	}
	catch (const MatchInProgress& e)
	{
		_userModel->SetOnCurrentGame(false);
		_userService->UpdateUser(_userModel);
		_gameService->RemoveGame(_gameModel);
	}
	catch (const GameInvalid& ex)
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

	if (_userModel->GetNickName().compare("") == 0)
	{
		throw GameInvalid("O usu�rio n�o pode criar um jogo com Nickname vazio!");
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
		throw GameInvalid("O usu�rio atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
	}

	if (_userModel->GetOnCurrentGame())
	{
		throw GameInvalid("O usu�rio est� atualmente ativo em uma partida!");
	}
}

void MenuController::CreateConnection(const bool& createGame)
{
	wstring valuePassword = Serialize::ConvertStringToWString(_gameModel->GetId());

	shared_future<bool> isPartnerConnected = async(launch::async, [this, valuePassword, createGame]()
		{
			_communicationService->SetPipePassword(valuePassword);
			bool getOpenedChannel = _communicationService->OpenCommunicationChannel(valuePassword);

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

				throw GameInvalid("Ocorreu um problema na criação do game, tente novamente!");
			}

			StructMessage receivedvalue = _communicationService->ReceiveDataFromPipe();
			if (receivedvalue.MessageSuccessfuly)
			{
				StartGame(receivedvalue.MessageSuccessfuly, _gameService, _gameModel);

				StructMessage messageSender;
				messageSender.MessageSuccessfuly = true;
				messageSender.Content = Serialize::ConvertGameModelToString(_gameModel);
				_communicationService->SendDataToPipe(messageSender);

				return getOpenedChannel;
			}

			return false;
		});

	_gameService->MonitoringPartnerConnection(isPartnerConnected);
}

void MenuController::ConnectionChannel(const bool& joinGame)
{
	wstring valuePassword = Serialize::ConvertStringToWString(_gameModel->GetId());

	shared_future<bool> isPartnerConnected = async(launch::async, [this, valuePassword, joinGame]()
		{
			_communicationService->SetPipePassword(valuePassword);
			bool getConnectChannel = _communicationService->ConnectChannel(valuePassword);

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

				//throw GameInvalid("Ocorreu um problema ao entrar no game, tente novamente!");
			}
			StructMessage message;
			message.MessageSuccessfuly = true;
			message.Content = "Successfuly Client Connect?";
			bool messageSent = _communicationService->SendMessageClient(message);

			if (messageSent)
			{
				StructMessage messageReceived = _communicationService->ReceiveMessageClient();
				//Tá dando Erro aqui!!!
				//StartGameJoinGame(messageReceived, _gameModel);

				return getConnectChannel;
			}

			return false;
		});

	_gameService->MonitoringPartnerConnection(isPartnerConnected);
}

void MenuController::StartGame(const bool& messageSuccessfuly, shared_ptr<GameService>& gameService, shared_ptr<GameModel>& gameModel)
{
	if (messageSuccessfuly)
	{
		gameService->StartGame(gameModel);
		gameService->Hand(gameModel);
	}
	else
	{
		throw GameInvalid("O jogador n�o conseguiu se conectar na partida!");
	}
}

void MenuController::StartGameJoinGame(StructMessage response, shared_ptr<GameModel>& gameModel)
{
	if (response.MessageSuccessfuly)
	{
		std::cout << response.Content << std::endl;
		gameModel->CopyFrom(Serialize::ConvertStringToGameModel(response.Content));
	}
	else
	{
		throw GameInvalid("O host n�o conseguiu inicializar a partida!");
	}
}
