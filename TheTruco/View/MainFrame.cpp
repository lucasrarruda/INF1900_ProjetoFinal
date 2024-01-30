#include "pch.h"
#include "MainFrame.h"

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

MainFrame::MainFrame() : _menuView(this), _gameView(this), _newGameView(this), _joinGameView(this)
{
	
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
	if (!CFrameWnd::Create(NULL, _T("The Truco"), WS_OVERLAPPED | WS_SYSMENU, CRect(100, 100, 1280, 1280)))
		return FALSE;

	ShowWindow(SW_SHOW);
	UpdateWindow();

	_menuView.Create();

	_newGameView.Create();
	_joinGameView.Create();
	_gameView.Create();
	_newGameView.Hide();
	_joinGameView.Hide();
	_gameView.Hide();

	return TRUE;
}

void MainFrame::NavigateNewGame()
{
	HideAllViews();
	_newGameView.Show();
	UpdateFrame();
}

void MainFrame::NavigateJoinGame()
{
	HideAllViews();
	_joinGameView.Show();
	UpdateFrame();
}

void MainFrame::NavigatePlayGame()
{
	HideAllViews();
	_gameView.Show();
	UpdateFrame();
}

void MainFrame::NavigateMenu()
{
	HideAllViews();
	_menuView.Show();
	UpdateFrame();
}


void MainFrame::DropCardOne()
{
	_gameView.DropCardOne();
	UpdateFrame();
}

void MainFrame::DropCardTwo()
{
	_gameView.DropCardTwo();
	UpdateFrame();
}

void MainFrame::DropCardThree()
{
	_gameView.DropCardThree();
	UpdateFrame();
}

void MainFrame::NotifyTruco()
{
	_gameView.NotifyTruco();
	UpdateFrame();
}

void MainFrame::HideAllViews()
{
	_menuView.Hide();
	_gameView.Hide();
	_newGameView.Hide();
	_joinGameView.Hide();
}

void MainFrame::UpdateFrame()
{
	ShowWindow(SW_HIDE);
	UpdateWindow();
	ShowWindow(SW_SHOW);
	UpdateWindow();
}
