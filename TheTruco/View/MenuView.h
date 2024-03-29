#pragma once

#include <Interfaces/ViewBase.h>
#include <MenuController.h>
#include <memory>

class MenuView: public Interfaces::ViewBase, public CFrameWnd, public std::enable_shared_from_this<MenuView>
{
public:
	MenuView(CWnd* parentWindow, std::shared_ptr<Controller::MenuController> menuController);
	~MenuView() = default;

	void Create();
	void Update();
	void Show();
	void Hide();

	void NewGame();
	void JoinGame();
	void RecoverLast();

private:
	std::shared_ptr<Controller::MenuController> _menuController;
	std::shared_ptr<Model::UserModel> _userModel;

	CWnd* _parentWindow;
	CStatic _labelTitle;
	CStatic _labelNickName;
	CEdit _nickNameBox;
	CButton _newGameButton;
	CButton _joinGameButton;
	CButton _recoverLastGameButton;

	CFont _titleFont;
	CFont _nicknameFont;
	CFont _labelFont;
	CFont _buttonFont;

	void UpdateNickname();
};

