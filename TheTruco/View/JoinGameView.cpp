#include "pch.h"
#include <JoinGameView.h>
#include <DisplayHelper.h>

using namespace std;

JoinGameView::JoinGameView(CWnd* parentWindow, std::shared_ptr<Controller::MenuController> menuController) : 
	Interfaces::ViewBase(), 
	_parentWindow(parentWindow),
	_menuController(menuController)
{
	_userModel = _menuController->GetUserModel();
}

void JoinGameView::Create()
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
	CRect labelGameCodeRect
	{
		static_cast<int>(330 * dpiX),
		static_cast<int>(500 * dpiY),
		static_cast<int>(500 * dpiX),
		static_cast<int>(530 * dpiY)
	};
	_labelGameCode.Create(_T("Type game code:"), WS_CHILD | WS_VISIBLE | SS_LEFT | BS_CENTER, labelGameCodeRect, _parentWindow);
	_labelGameCode.SetFont(&_labelFont);
	
	_gameCodeFont.CreatePointFont(static_cast<int>(150 * dpiX), _T("Arial"));
	CRect gameCodeBoxRect
	{
		static_cast<int>(330 * dpiX),
		static_cast<int>(530 * dpiY),
		static_cast<int>(810 * dpiX),
		static_cast<int>(580 * dpiY)
	};
	_gameCodeBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, gameCodeBoxRect, _parentWindow, IDC_GAME_CODE_BOX);
	_gameCodeBox.SetFont(&_gameCodeFont);

	_buttonFont.CreatePointFont(static_cast<int>(140 * dpiX), _T("Arial"));
	CRect startGameButtonRect
	{
		static_cast<int>(460 * dpiX),
		static_cast<int>(600 * dpiY),
		static_cast<int>(690 * dpiX),
		static_cast<int>(650 * dpiY)
	};
	_joinGameButton.Create(_T("Join Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, startGameButtonRect, _parentWindow, IDC_START_JOIN_GAME_BUTTON);
	_joinGameButton.SetFont(&_buttonFont);

	CRect backMenuButtonRect
	{
		static_cast<int>(460 * dpiX),
		static_cast<int>(670 * dpiY),
		static_cast<int>(690 * dpiX),
		static_cast<int>(720 * dpiY)
	};
	_backMenuButton.Create(_T("Back"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, backMenuButtonRect, _parentWindow, IDC_BACK_BUTTON_JOIN_GAME);
	_backMenuButton.SetFont(&_buttonFont);
}

void JoinGameView::Update()
{
	// Update UI with new model values
}

void JoinGameView::Show()
{
	_labelTitle.ShowWindow(SW_SHOW);
	_labelGameCode.ShowWindow(SW_SHOW);
	_gameCodeBox.ShowWindow(SW_SHOW);
	_joinGameButton.ShowWindow(SW_SHOW);
	_backMenuButton.ShowWindow(SW_SHOW);
}

void JoinGameView::Hide()
{
	_labelTitle.ShowWindow(SW_HIDE);
	_labelGameCode.ShowWindow(SW_HIDE);
	_gameCodeBox.ShowWindow(SW_HIDE);
	_joinGameButton.ShowWindow(SW_HIDE);
	_backMenuButton.ShowWindow(SW_HIDE);
}

void JoinGameView::StartJoinGameCommand()
{
}
