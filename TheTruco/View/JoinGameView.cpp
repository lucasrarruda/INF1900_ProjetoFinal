#include "pch.h"
#include "JoinGameView.h"

using namespace std;

JoinGameView::JoinGameView(CWnd* parentWindow) : ViewBase(), _parentWindow(parentWindow)
{
}

void JoinGameView::Create()
{
	_titleFont.CreatePointFont(250, _T("Arial"));
	_labelTitle.Create(_T("The Truco"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(420, 350, 740, 430), _parentWindow);
	_labelTitle.SetFont(&_titleFont);

	_labelGameCode.Create(_T("Type game code:"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(330, 500, 500, 520), _parentWindow);
	
	_gameCodeFont.CreatePointFont(150, _T("Arial"));
	_gameCodeBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(330, 530, 810, 580), _parentWindow, IDC_GAME_CODE_BOX);
	_gameCodeBox.SetFont(&_gameCodeFont);

	_startGameButton.Create(_T("Start Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(460, 600, 690, 650), _parentWindow, IDC_START_JOIN_GAME_BUTTON);
	_backMenuButton.Create(_T("Back"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(460, 670, 690, 720), _parentWindow, IDC_BACK_BUTTON_JOIN_GAME);
}

void JoinGameView::Updated()
{
	// Update UI with new model values
}

void JoinGameView::Show()
{
	_labelTitle.ShowWindow(SW_SHOW);
	_labelGameCode.ShowWindow(SW_SHOW);
	_gameCodeBox.ShowWindow(SW_SHOW);
	_startGameButton.ShowWindow(SW_SHOW);
	_backMenuButton.ShowWindow(SW_SHOW);
}

void JoinGameView::Hide()
{
	_labelTitle.ShowWindow(SW_HIDE);
	_labelGameCode.ShowWindow(SW_HIDE);
	_gameCodeBox.ShowWindow(SW_HIDE);
	_startGameButton.ShowWindow(SW_HIDE);
	_backMenuButton.ShowWindow(SW_HIDE);
}