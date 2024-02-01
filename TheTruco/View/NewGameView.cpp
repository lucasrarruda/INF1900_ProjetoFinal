#include "pch.h"
#include <NewGameView.h>

using namespace std;

NewGameView::NewGameView(CWnd* parentWindow) : Interfaces::ViewBase(), _parentWindow(parentWindow)
{
}

void NewGameView::Create()
{
	_titleFont.CreatePointFont(250, _T("Arial"));
	_labelTitle.Create(_T("The Truco"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(420, 350, 740, 430), _parentWindow);
	_labelTitle.SetFont(&_titleFont);

	_labelGameCode.Create(_T("Game code:"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(330, 500, 810, 550), _parentWindow);
	
	_gameCodeFont.CreatePointFont(150, _T("Arial"));
	_GameCode.Create(_T("xYr489"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(330, 530, 810, 580), _parentWindow);
	_GameCode.SetFont(&_gameCodeFont);

	_labelPlayerOne.Create(_T("Player 1 (You)"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(420, 600, 500, 645), _parentWindow);
	_labelPlayerTwo.Create(_T("Player 2 (Off)"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(510, 600, 580, 645), _parentWindow);
	_labelPlayerThree.Create(_T("Player 3 (Off)"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(590, 600, 660, 645), _parentWindow);
	_labelPlayerFour.Create(_T("Player 4 (Off)"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(670, 600, 740, 645), _parentWindow);

	_startGameButton.Create(_T("Start Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(460, 700, 690, 750), _parentWindow, IDC_START_NEW_GAME_BUTTON);
	_backMenuButton.Create(_T("Back"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(460, 770, 690, 820), _parentWindow, IDC_BACK_BUTTON_NEW_GAME);
}

void NewGameView::Updated()
{
	// Update UI with new model values
}

void NewGameView::Show()
{
	_labelTitle.ShowWindow(SW_SHOW);
	_labelGameCode.ShowWindow(SW_SHOW);
	_GameCode.ShowWindow(SW_SHOW);
	_labelPlayerOne.ShowWindow(SW_SHOW);
	_labelPlayerTwo.ShowWindow(SW_SHOW);
	_labelPlayerThree.ShowWindow(SW_SHOW);
	_labelPlayerFour.ShowWindow(SW_SHOW);
	_startGameButton.ShowWindow(SW_SHOW);
	_backMenuButton.ShowWindow(SW_SHOW);
}

void NewGameView::Hide()
{
	_labelTitle.ShowWindow(SW_HIDE);
	_labelGameCode.ShowWindow(SW_HIDE);
	_GameCode.ShowWindow(SW_HIDE);
	_labelPlayerOne.ShowWindow(SW_HIDE);
	_labelPlayerTwo.ShowWindow(SW_HIDE);
	_labelPlayerThree.ShowWindow(SW_HIDE);
	_labelPlayerFour.ShowWindow(SW_HIDE);
	_startGameButton.ShowWindow(SW_HIDE);
	_backMenuButton.ShowWindow(SW_HIDE);
}	