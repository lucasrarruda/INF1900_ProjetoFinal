#pragma once

#include <Interfaces/ViewBase.h>
#include <MenuController.h>
#include <memory>

class MenuView: public Interfaces::ViewBase, public CFrameWnd, public std::enable_shared_from_this<MenuView>
{
public:
	MenuView(CWnd* parentWindow, std::shared_ptr<MenuController> menuController);
	~MenuView() = default;

	void Create();
	void Updated();
	void Show();
	void Hide();

	void NewGameCommand();
	void JoinGameCommand();

private:
	std::shared_ptr<MenuController> _menuController;
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

	void InitializeController();
	void UpdateNickname();
};

