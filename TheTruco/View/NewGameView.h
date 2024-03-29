#pragma once
#include <Interfaces/ViewBase.h>
#include <MenuController.h>

class NewGameView : public Interfaces::ViewBase, public CFrameWnd, public std::enable_shared_from_this<NewGameView>
{
public:
	NewGameView(CWnd* parentWindow, std::shared_ptr<Controller::MenuController> menuController);
	~NewGameView() = default;

	void Create();
	void Update();
	void Show();
	void Hide();

private:
	std::shared_ptr<Controller::MenuController> _menuController;
	std::shared_ptr<Model::UserModel> _userModel;

	CWnd* _parentWindow;
	CStatic _labelTitle;
	CStatic _labelGameCode;
	CStatic _gameCode;
	CStatic _labelPlayerPartner;
	CButton _startGameButton;
	CButton _backMenuButton;

	CFont _titleFont;
	CFont _gameCodeFont;
	CFont _labelFont;
	CFont _buttonFont;
};

