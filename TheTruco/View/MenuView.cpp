#include "pch.h"
#include <MenuView.h>
#include <MenuController.h>
#include <DisplayHelper.h>
#include <GeneralHelper.h>

using namespace std;

MenuView::MenuView(CWnd* parentWindow, std::shared_ptr<Controller::MenuController> menuController) :
	Interfaces::ViewBase(), 
	_parentWindow(parentWindow),
	_menuController(menuController)
{
	_userModel = _menuController->GetUserModel();
	//EXEMPLO DE VALIDAÇÃO DOS MÉTODOS
	
	//auto testeconexao = make_shared<Communication::CommunicationService>();
	//auto teste = make_shared<Controller::MenuController>(testeconexao);
	//auto teste2 = make_shared<Controller::MenuController>(testeconexao);

	//teste->NewGame("ARTHUR");
	//teste2->JoinGame("JOAO", teste->GetGameModel().GetId());
	//teste->RecoverLastGame("LAURA");
	//teste->GetUserModel().SetOnCurrentGame(false);
	//teste->RecoverLastGame("ARTHUR");
}

void MenuView::Create()
{
	_userModel->Attach(shared_from_this());

	auto [dpiX, dpiY] = DisplayHelper::GetMonitorDpi();

	_titleFont.CreatePointFont(static_cast<int>(380 * dpiX), _T("Arial"));
	CRect titleRect
	{
		static_cast<int>(420 * dpiX),
		static_cast<int>(350 * dpiY),
		static_cast<int>(740 * dpiX),
		static_cast<int>(430 * dpiY)
	};
	_labelTitle.Create(_T("The Truco"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, titleRect, _parentWindow);
	_labelTitle.SetFont(&_titleFont);

	_labelFont.CreatePointFont(static_cast<int>(120 * dpiX), _T("Arial"));
	CRect nicknameRect
	{
		static_cast<int>(330 * dpiX),
		static_cast<int>(500 * dpiY),
		static_cast<int>(530 * dpiX),
		static_cast<int>(530 * dpiY)
	};
	_labelNickName.Create(_T("Type your nickname:"), WS_CHILD | WS_VISIBLE | SS_LEFT | BS_CENTER, nicknameRect, _parentWindow);
	_labelNickName.SetFont(&_labelFont);
	
	_nicknameFont.CreatePointFont(static_cast<int>(150 * dpiX), _T("Arial"));
	CRect nicknameBoxRect
	{
		static_cast<int>(330 * dpiX),
		static_cast<int>(530 * dpiY),
		static_cast<int>(810 * dpiX),
		static_cast<int>(580 * dpiY)
	};
	_nickNameBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, nicknameBoxRect, _parentWindow, IDC_NICKNAME_BOX);
	_nickNameBox.SetFont(&_nicknameFont);

	_buttonFont.CreatePointFont(static_cast<int>(140 * dpiX), _T("Arial"));
	CRect newGameButtonRect
	{
		static_cast<int>(460 * dpiX),
		static_cast<int>(600 * dpiY),
		static_cast<int>(690 * dpiX),
		static_cast<int>(650 * dpiY)
	};
	_newGameButton.Create(_T("New Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, newGameButtonRect, _parentWindow, IDC_NEW_GAME_BUTTON);
	_newGameButton.SetFont(&_buttonFont);

	CRect joinGameButtonRect
	{
		static_cast<int>(460 * dpiX),
		static_cast<int>(670 * dpiY),
		static_cast<int>(690 * dpiX),
		static_cast<int>(720 * dpiY)
	};
	_joinGameButton.Create(_T("Join Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, joinGameButtonRect, _parentWindow, IDC_JOIN_GAME_BUTTON);
	_joinGameButton.SetFont(&_buttonFont);

	CRect recoverLastGameButtonRect
	{
		static_cast<int>(460 * dpiX),
		static_cast<int>(740 * dpiY),
		static_cast<int>(690 * dpiX),
		static_cast<int>(790 * dpiY)
	};
	_recoverLastGameButton.Create(_T("Recover Last Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, recoverLastGameButtonRect, _parentWindow, IDC_RECOVER_LAST_GAME_BUTTON);
	_recoverLastGameButton.SetFont(&_buttonFont);
}

void MenuView::Updated()
{
	// Update UI with new model values
}

void MenuView::Show()
{
	_labelTitle.ShowWindow(SW_SHOW);
	_labelNickName.ShowWindow(SW_SHOW);
	_nickNameBox.ShowWindow(SW_SHOW);
	_newGameButton.ShowWindow(SW_SHOW);
	_joinGameButton.ShowWindow(SW_SHOW);
	_recoverLastGameButton.ShowWindow(SW_SHOW);
}

void MenuView::Hide()
{
	_labelTitle.ShowWindow(SW_HIDE);
	_labelNickName.ShowWindow(SW_HIDE);
	_nickNameBox.ShowWindow(SW_HIDE);
	_newGameButton.ShowWindow(SW_HIDE);
	_joinGameButton.ShowWindow(SW_HIDE);
	_recoverLastGameButton.ShowWindow(SW_HIDE);
}

void MenuView::NewGameCommand()
{
	UpdateNickname();
	_menuController->NewGame();
}

void MenuView::JoinGameCommand()
{
	UpdateNickname();
}

void MenuView::UpdateNickname()
{
	CString nickname;
	_nickNameBox.GetWindowTextW(nickname);
	_userModel->SetNickName(GeneralHelper::CStringToString(nickname));
}
