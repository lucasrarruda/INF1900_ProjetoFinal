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

void Controller::GameController::WaitConnetion()
{
}

void Controller::GameController::CopyGameCodetoClipboard(const wstring& gameCode)
{
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();

        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (gameCode.length() + 1) * sizeof(wchar_t));
        if (hMem != nullptr) {
            wchar_t* pMem = static_cast<wchar_t*>(GlobalLock(hMem));
            if (pMem != nullptr) {
                wcscpy_s(pMem, gameCode.length() + 1, gameCode.c_str());

                // Libera a memória global
                GlobalUnlock(hMem);

                // Define o formato de dados na área de transferência
                SetClipboardData(CF_UNICODETEXT, hMem);
            }
            else {
                // TODO: tratar exceção na interface
                std::cerr << "Falha ao travar a memória global." << std::endl;
            }
        }
        else {
            // TODO: tratar exceção na interface
            std::cerr << "Falha ao alocar memória global." << std::endl;
        }

        // Fecha a área de transferência
        CloseClipboard();
    }
    else {
        // TODO: tratar exceção na interface
        std::cerr << "Falha ao abrir a área de transferência." << std::endl;
    }
}

void Controller::GameController::NotifyTruco()
{
    _gameService->Truco(_gameModel);
    _gameModel->SetTurnPlayer(3);
    _gameService->UpdateOtherPlayers(_gameModel);
    _gameModel->SwitchTurn();
}

void Controller::GameController::LeaveGame()
{
    // TODO:
}

void Controller::GameController::PlayCard(const int& id)
{
    _gameService->PlayCard(_gameModel, _userModel->GetNickName(), id);
}

void Controller::GameController::PlayCoveredCard(const int& id)
{
    _gameService->HideCard(_gameModel, _userModel->GetNickName(), id);
}
