#pragma once
#include <afxwin.h>
#include <MenuView.h>
#include <JoinGameView.h>
#include <GameView.h>
#include <tuple>
#include <memory>
#include <MenuController.h>

class MainFrame : public CFrameWnd
{
public:
	MainFrame();

	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL Create();

	void CreateNewGame();
	void JoinGame();
	void StartJoinGame();
	void RecoverLastGame();
	void LeaveGame();
	void BackToMenu();
	void CopyGameCodeToClipboard();
	void DropCardOne();
	void DropCardTwo();
	void DropCardThree();
	void NotifyTruco();

	DECLARE_MESSAGE_MAP()

private:
	std::shared_ptr<MenuView> _menuView;
	std::shared_ptr<JoinGameView> _joinGameView;
	std::shared_ptr<GameView> _gameView;
	std::shared_ptr<Communication::CommunicationService> _communicationService;
	std::shared_ptr<Controller::MenuController> _menuController;
	std::shared_ptr<Controller::GameController> _gameController;

	void HideAllViews();
	void UpdateFrame();
};

