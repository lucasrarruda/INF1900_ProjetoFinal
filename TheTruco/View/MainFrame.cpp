#include "pch.h"
#include <MainFrame.h>
#include <DisplayHelper.h>

using namespace std;
using namespace Communication;
using namespace Controller;

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
	ON_COMMAND(IDC_NEW_GAME_BUTTON, CreateNewGame)
	ON_COMMAND(IDC_JOIN_GAME_BUTTON, JoinGame)
	ON_COMMAND(IDC_START_JOIN_GAME_BUTTON, StartJoinGame)
	ON_COMMAND(IDC_LEAVE_GAME_BUTTON, LeaveGame)
	ON_COMMAND(IDC_BACK_BUTTON_JOIN_GAME, BackToMenu)
	ON_COMMAND(IDC_COPY_TO_CLIPBOARD_BUTTON, CopyGameCodeToClipboard)
	ON_COMMAND(IDC_YOUR_CARD_ONE_BUTTON, DropCardOne)
	ON_COMMAND(IDC_YOUR_CARD_TWO_BUTTON, DropCardTwo)
	ON_COMMAND(IDC_YOUR_CARD_THREE_BUTTON, DropCardThree)
	ON_COMMAND(IDC_TRUCO_BUTTON, NotifyTruco)
	ON_COMMAND(IDC_RECOVER_LAST_GAME_BUTTON, RecoverLastGame)
END_MESSAGE_MAP()

MainFrame::MainFrame()
{
	_contentProvider = make_shared<ContentProvider>();
	_menuController = make_shared<MenuController>(_contentProvider);
	_gameController = make_shared<GameController>(_contentProvider);

	_menuView = make_shared<MenuView>(this, _menuController);
	_joinGameView = make_shared<JoinGameView>(this, _menuController);
	_gameView = make_shared<GameView>(this, _gameController);
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

BOOL MainFrame::Create()
{
	auto [dpiX, dpiY] = DisplayHelper::GetMonitorDpi();
	if (dpiX >= 1.0)
		dpiX >= 0.8;
	if (dpiY == 1.0)
		dpiY = 0.8;

	if (!CFrameWnd::Create(NULL, _T("The Truco"), WS_OVERLAPPED | WS_SYSMENU, CRect(100, 100, static_cast<int>(1280 * dpiX), static_cast<int>(1280 * dpiY))))
		return FALSE;

	ShowWindow(SW_SHOW);
	UpdateWindow();

	_menuView->Create();
	_joinGameView->Create();
	_gameView->Create();
	_joinGameView->Hide();
	_gameView->Hide();
	UpdateFrame();

	return TRUE;
}

void MainFrame::CreateNewGame()
{
	HideAllViews();
	_menuView->NewGame();
	_gameView->Show();
	UpdateFrame();
}

void MainFrame::JoinGame()
{
	HideAllViews();
	_menuView->JoinGame();
	_joinGameView->Show();
	UpdateFrame();
}

void MainFrame::StartJoinGame()
{
	HideAllViews();
	_joinGameView->StartJoinGame();
	_gameView->Show();
	UpdateFrame();
}

void MainFrame::RecoverLastGame()
{
	HideAllViews();
	_menuView->RecoverLast();
	_gameView->Show();
	UpdateFrame();
}

void MainFrame::LeaveGame()
{
	HideAllViews();
	_gameView->LeaveGame();
	_menuView->Show();
	UpdateFrame();
}

void MainFrame::BackToMenu()
{
	HideAllViews();
	_joinGameView->BackCommand();
	_menuView->Show();
	UpdateFrame();
}

void MainFrame::CopyGameCodeToClipboard()
{
	_gameView->CopyGameCodeToClipboard();
}


void MainFrame::DropCardOne()
{
	_gameView->DropCardOne();
	UpdateFrame();
}

void MainFrame::DropCardTwo()
{
	_gameView->DropCardTwo();
	UpdateFrame();
}

void MainFrame::DropCardThree()
{
	_gameView->DropCardThree();
	UpdateFrame();
}

void MainFrame::NotifyTruco()
{
	_gameView->NotifyTruco();
	UpdateFrame();
}

void MainFrame::HideAllViews()
{
	_menuView->Hide();
	_joinGameView->Hide();
	_gameView->Hide();
}

void MainFrame::UpdateFrame()
{
	ShowWindow(SW_HIDE);
	UpdateWindow();
	ShowWindow(SW_SHOW);
	UpdateWindow();
}