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
			throw MatchInProgress("O usuario atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
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
			throw MatchInProgress("O usuario atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
		}

		_userModel->SetOnCurrentGame(true);

		_userService->UpdateUser(_userModel);

		bool isPlayerHost = _gameModel->IsHostPlayer(_userModel->GetNickName());

		if (isPlayerHost)
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
		throw GameInvalid("O usuario nao pode criar um jogo com Nickname vazio!");
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
		throw GameInvalid("O usuario atualmente tem uma partida em andamento, finalize a partida para poder criar outra!");
	}

	if (_userModel->GetOnCurrentGame())
	{
		throw GameInvalid("O usuario esta atualmente ativo em uma partida!");
	}
}

void MenuController::CreateConnection(const bool& createGame)
{
	wstring valuePassword = Serialize::ConvertStringToWString(_gameModel->GetId());

	shared_future<bool> isPartnerConnected = async(launch::async, [this, valuePassword, createGame]()
		{
			try
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

				return getOpenedChannel;
			}
			catch (const GameInvalid&)
			{
				_userModel->SetCurrentGameID("");
				_userModel->SetOnCurrentGame(false);
				_userService->UpdateUser(_userModel);
				_gameService->RemoveGame(_gameModel);

				return false;
			} 
		});

	_gameService->MonitoringPartnerConnection(isPartnerConnected, _gameModel);
}

void MenuController::ConnectionChannel(const bool& joinGame)
{
	wstring valuePassword = Serialize::ConvertStringToWString(_gameModel->GetId());

	shared_future<bool> isPartnerConnected = async(launch::async, [this, valuePassword, joinGame]()
		{
			try
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

					throw GameInvalid("Ocorreu um problema ao entrar no game, tente novamente!");
				}

				return getConnectChannel;
			}
			catch (const GameInvalid&)
			{
				_userModel->SetCurrentGameID("");
				_userModel->SetOnCurrentGame(false);
				_userService->UpdateUser(_userModel);
				_gameService->LeaveGame(_gameModel, _userModel->GetNickName());

				return false;
			}
		});

	_gameService->MonitoringPartnerConnection(isPartnerConnected, _gameModel);
}
