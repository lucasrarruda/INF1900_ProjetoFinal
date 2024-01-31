#pragma once

#include <Interfaces/ViewBase.h>
#include <GameModelFake.h>
#include <memory>

class MenuView: public Interfaces::ViewBase, public CFrameWnd
{
public:
	MenuView(CWnd* parentWindow);
	~MenuView() = default;

	void Create();
	void Updated();
	void Show();
	void Hide();

private:
	std::shared_ptr<GameModelFake> _gameModel;

	CWnd* _parentWindow;
	CStatic _labelTitle;
	CStatic _labelNickName;
	CEdit _nickNameBox;
	CButton _newGameButton;
	CButton _joinGameButton;
	CButton _recoverLastGame;

	CFont _titleFont;
	CFont _nicknameFont;
};

