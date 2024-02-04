#pragma once
#include <Interfaces/ViewBase.h>
#include <MenuController.h>

class JoinGameView : public Interfaces::ViewBase, public CFrameWnd, public std::enable_shared_from_this<JoinGameView>
{
public:
	JoinGameView(CWnd* parentWindow, std::shared_ptr<Controller::MenuController> menuController);
	~JoinGameView() = default;

	void Create();
	void Update();
	void Show();
	void Hide();

	void StartJoinGameCommand();

private:
	std::shared_ptr<Controller::MenuController> _menuController;
	std::shared_ptr<Model::UserModel> _userModel;

	CWnd* _parentWindow;
	CStatic _labelTitle;
	CStatic _labelGameCode;
	CEdit _gameCodeBox;
	CButton _joinGameButton;
	CButton _backMenuButton;

	CFont _titleFont;
	CFont _gameCodeFont;
	CFont _labelFont;
	CFont _buttonFont;
};

