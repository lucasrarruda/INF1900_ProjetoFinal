#pragma once
#include <afxwin.h>
#include <MenuView.h>
#include <GameView.h>
#include <JoinGameView.h>
#include <NewGameView.h>
#include <tuple>
#include <memory>
#include <UserModel.h>

class MainFrame : public CFrameWnd
{
public:
	MainFrame();

	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL Create();

	void NavigateNewGame();
	void NavigateJoinGame();
	void NavigatePlayGame();
	void NavigateMenu();

	void DropCardOne();
	void DropCardTwo();
	void DropCardThree();
	void NotifyTruco();

	DECLARE_MESSAGE_MAP()

private:
	MenuView _menuView;
	GameView _gameView;
	NewGameView _newGameView;
	JoinGameView _joinGameView;

	std::shared_ptr<Model::UserModel> _userModel;

	void HideAllViews();
	void UpdateFrame();
};

