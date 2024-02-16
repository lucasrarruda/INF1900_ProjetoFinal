#include "pch.h"
#include <GameController.h>
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace Model;

Controller::GameController::GameController(const std::shared_ptr<Controller::ContentProvider>& contentProvider):
	CommunicationService(contentProvider->CommunicationServiceInstance)
{
	_gameService = contentProvider->GameServiceInstance;
	_userService = contentProvider->UserServiceInstance;

    _userModel = contentProvider->UserModelInstance;
    _gameModel = contentProvider->GameModelInstance;
    _currentPlayerModel = contentProvider->CurrentPlayerModel;
}

void Controller::GameController::CopyGameCodetoClipboard(const wstring& gameCode)
{
    try
    {
        if (OpenClipboard(nullptr)) 
        {
            EmptyClipboard();

            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (gameCode.length() + 1) * sizeof(wchar_t));
            if (hMem != nullptr) 
            {
                wchar_t* pMem = static_cast<wchar_t*>(GlobalLock(hMem));
                if (pMem != nullptr) 
                {
                    wcscpy_s(pMem, gameCode.length() + 1, gameCode.c_str());

                    // Libera a memória global
                    GlobalUnlock(hMem);

                    // Define o formato de dados na área de transferência
                    SetClipboardData(CF_UNICODETEXT, hMem);
                }
                else 
                {
                    throw "Falha ao travar a memória global.";
                }
            }
            else 
            {
                throw "Falha ao alocar memória global.";
            }

            // Fecha a área de transferência
            CloseClipboard();
        }
        else 
        {
           
            throw "Falha ao abrir a área de transferência.";
        }
    }
    catch (const std::exception&)
    {
        // TODO: tratar exceção na interface
    }
}

void Controller::GameController::NotifyTruco()
{
    _gameService->Truco(_gameModel);
    _gameService->UpdateOtherPlayers(_gameModel);
    _gameModel->SwitchTurn();
}

void Controller::GameController::LeaveGame(const bool& otherPlayer)
{
    if (!otherPlayer)
    {
        _userModel->SetCurrentGameID("");
        _userModel->SetOnCurrentGame(false);
    }
    
    _userService->UpdateUser(_userModel);
    _gameService->SurrenderGame(_gameModel);
    _gameModel->SetTurnPlayer(3);
    _gameModel->SetLeaveGame(true);

    if (!otherPlayer)
    {
        if (_gameModel->GetPlayers().size() == 4)
        {
            _gameService->UpdateOtherPlayers(_gameModel);  
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        _gameService->GetCommunicationService()->CloseConnection();
        _gameService->KillWaitingThread();
    }
    else
    {
        _gameService->KillWaitingThread();
    }

    _gameService->ResetGame(_gameModel);
}

void Controller::GameController::PlayCard(const int& id)
{
    _gameService->PlayCard(_gameModel, _userModel->GetNickName(), id);
}

void Controller::GameController::PlayCoveredCard(const int& id)
{
    _gameService->HideCard(_gameModel, _userModel->GetNickName(), id);
}

void Controller::GameController::LeaveClient()
{
    _userModel->SetCurrentGameID("");
    _userModel->SetOnCurrentGame(false);
    _userService->UpdateUser(_userModel);
}
