#include "pch.h"
#include <NewGameView.h>
#include <DisplayHelper.h>

using namespace std;

NewGameView::NewGameView(CWnd* parentWindow) : Interfaces::ViewBase(), _parentWindow(parentWindow)
{
}

void NewGameView::Create()
{
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
		static_cast<int>(810 * dpiX),
		static_cast<int>(530 * dpiY)
	};
	_labelGameCode.Create(_T("Game code:"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, labelGameCodeRect, _parentWindow);
	_labelGameCode.SetFont(&_labelFont);
	
	_gameCodeFont.CreatePointFont(static_cast<int>(170 * dpiX), _T("Arial"));
	CRect gameCodeRect
	{
		static_cast<int>(330 * dpiX),
		static_cast<int>(540 * dpiY),
		static_cast<int>(810 * dpiX),
		static_cast<int>(590 * dpiY)
	};
	_gameCode.Create(_T("xYr489"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, gameCodeRect, _parentWindow);
	_gameCode.SetFont(&_gameCodeFont);

	CRect labelPlayerPartnerRect
	{
		static_cast<int>(450 * dpiX),
		static_cast<int>(600 * dpiY),
		static_cast<int>(700 * dpiX),
		static_cast<int>(630 * dpiY)
	};
	_labelPlayerPartner.Create(_T("Waiting Partner..."), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, labelPlayerPartnerRect, _parentWindow);

	CRect startGameRect
	{
		static_cast<int>(460 * dpiX),
		static_cast<int>(700 * dpiY),
		static_cast<int>(690 * dpiX),
		static_cast<int>(750 * dpiY)
	};
	_startGameButton.Create(_T("Start Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, startGameRect, _parentWindow, IDC_START_NEW_GAME_BUTTON);
	
	CRect backMenuRect
	{
		static_cast<int>(460 * dpiX),
		static_cast<int>(770 * dpiY),
		static_cast<int>(690 * dpiX),
		static_cast<int>(820 * dpiY)
	};
	_backMenuButton.Create(_T("Back"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, backMenuRect, _parentWindow, IDC_BACK_BUTTON_NEW_GAME);
}

void NewGameView::Updated()
{
	// Update UI with new model values
}

void NewGameView::Show()
{
	_labelTitle.ShowWindow(SW_SHOW);
	_labelGameCode.ShowWindow(SW_SHOW);
	_gameCode.ShowWindow(SW_SHOW);
	_labelPlayerPartner.ShowWindow(SW_SHOW);
	_startGameButton.ShowWindow(SW_SHOW);
	_backMenuButton.ShowWindow(SW_SHOW);
}

void NewGameView::Hide()
{
	_labelTitle.ShowWindow(SW_HIDE);
	_labelGameCode.ShowWindow(SW_HIDE);
	_gameCode.ShowWindow(SW_HIDE);
	_labelPlayerPartner.ShowWindow(SW_HIDE);
	_startGameButton.ShowWindow(SW_HIDE);
	_backMenuButton.ShowWindow(SW_HIDE);
}	