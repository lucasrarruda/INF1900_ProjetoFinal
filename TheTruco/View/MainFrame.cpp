#include "pch.h"
#include <MainFrame.h>
#include <DisplayHelper.h>

using namespace std;

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
	ON_COMMAND(IDC_NEW_GAME_BUTTON, NavigateNewGame)
	ON_COMMAND(IDC_JOIN_GAME_BUTTON, NavigateJoinGame)
	ON_COMMAND(IDC_BACK_BUTTON_JOIN_GAME, NavigateMenu)
	ON_COMMAND(IDC_BACK_BUTTON_NEW_GAME, NavigateMenu)
	ON_COMMAND(IDC_START_NEW_GAME_BUTTON, NavigatePlayGame)
	ON_COMMAND(IDC_START_JOIN_GAME_BUTTON, NavigatePlayGame)
	ON_COMMAND(IDC_YOUR_CARD_ONE_BUTTON, DropCardOne)
	ON_COMMAND(IDC_YOUR_CARD_TWO_BUTTON, DropCardTwo)
	ON_COMMAND(IDC_YOUR_CARD_THREE_BUTTON, DropCardThree)
	ON_COMMAND(IDC_TRUCO_BUTTON, NotifyTruco)
END_MESSAGE_MAP()

MainFrame::MainFrame()
{
	_communicationService = make_shared<Communication::CommunicationService>();
	_menuController = make_shared<Controller::MenuController>(_communicationService);
	_menuView = make_shared<MenuView>(this, _menuController);

	_newGameView = make_shared<NewGameView>(this, _menuController);
	_joinGameView = make_shared<JoinGameView>(this);
	_gameView = make_shared<GameView>(this);
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

	_newGameView->Create();
	_joinGameView->Create();
	_gameView->Create();
	_newGameView->Hide();
	_joinGameView->Hide();
	_gameView->Hide();
	UpdateFrame();

	return TRUE;
}

void MainFrame::NavigateNewGame()
{
	HideAllViews();
	_menuView->NewGameCommand();
	_newGameView->Show();
	UpdateFrame();
}

void MainFrame::NavigateJoinGame()
{
	HideAllViews();
	_joinGameView->Show();
	UpdateFrame();
}

void MainFrame::NavigatePlayGame()
{
	HideAllViews();
	_gameView->Show();
	UpdateFrame();
}

void MainFrame::NavigateMenu()
{
	HideAllViews();
	_menuView->Show();
	UpdateFrame();
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
	_gameView->Hide();
	_newGameView->Hide();
	_joinGameView->Hide();
}

void MainFrame::UpdateFrame()
{
	ShowWindow(SW_HIDE);
	UpdateWindow();
	ShowWindow(SW_SHOW);
	UpdateWindow();
}