#include "pch.h"
#include "MenuView.h"
#include <MenuControllerFake.h>

using namespace std;

MenuView::MenuView(CWnd* parentWindow) : ViewBase(), _parentWindow(parentWindow)
{
	_gameModel = make_shared<GameModelFake>();
	_controller = make_shared<MenuControllerFake>(_gameModel);
}

void MenuView::Create()
{
	_titleFont.CreatePointFont(250, _T("Arial"));
	_labelTitle.Create(_T("The Truco"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(420, 350, 740, 430), _parentWindow);
	_labelTitle.SetFont(&_titleFont);

	_labelNickName.Create(_T("Type your nickname:"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(330, 500, 500, 520), _parentWindow);
	
	_nicknameFont.CreatePointFont(150, _T("Arial"));
	_nickNameBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(330, 530, 810, 580), _parentWindow, IDC_NICKNAME_BOX);
	_nickNameBox.SetFont(&_nicknameFont);

	_newGameButton.Create(_T("New Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(460, 600, 690, 650), _parentWindow, IDC_NEW_GAME_BUTTON);
	_joinGameButton.Create(_T("Join Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(460, 670, 690, 720), _parentWindow, IDC_JOIN_GAME_BUTTON);
	_recoverLastGame.Create(_T("Recover Last Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(460, 740, 690, 790), _parentWindow, IDC_RECOVER_LAST_GAME_BUTTON);
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
	_recoverLastGame.ShowWindow(SW_SHOW);
}

void MenuView::Hide()
{
	_labelTitle.ShowWindow(SW_HIDE);
	_labelNickName.ShowWindow(SW_HIDE);
	_nickNameBox.ShowWindow(SW_HIDE);
	_newGameButton.ShowWindow(SW_HIDE);
	_joinGameButton.ShowWindow(SW_HIDE);
	_recoverLastGame.ShowWindow(SW_HIDE);
}
