#include "pch.h"
#include <GameController.h>
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace Model;

Controller::GameController::GameController(const std::shared_ptr<Communication::CommunicationService>& communicationService): 
	_communicationService(communicationService)
{
	//auto gameRepository = GameRepository();
	//auto userRepository = UserRepository();

	//_gameService = make_shared<GameService>(gameRepository);
	//_userService = make_shared<UserService>(userRepository);
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

                // Libera a mem�ria global
                GlobalUnlock(hMem);

                // Define o formato de dados na �rea de transfer�ncia
                SetClipboardData(CF_UNICODETEXT, hMem);
            }
            else {
                // TODO: tratar exce��o na interface
                std::cerr << "Falha ao travar a mem�ria global." << std::endl;
            }
        }
        else {
            // TODO: tratar exce��o na interface
            std::cerr << "Falha ao alocar mem�ria global." << std::endl;
        }

        // Fecha a �rea de transfer�ncia
        CloseClipboard();
    }
    else {
        // TODO: tratar exce��o na interface
        std::cerr << "Falha ao abrir a �rea de transfer�ncia." << std::endl;
    }
}