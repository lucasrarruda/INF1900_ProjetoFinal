#pragma once
#include <Interfaces/ViewBase.h>

class JoinGameView : public Interfaces::ViewBase, public CFrameWnd
{
public:
	JoinGameView(CWnd* parentWindow);
	~JoinGameView() = default;

	void Create();
	void Updated();
	void Show();
	void Hide();

private:
	CWnd* _parentWindow;
	CStatic _labelTitle;
	CStatic _labelGameCode;
	CEdit _gameCodeBox;
	CButton _startGameButton;
	CButton _backMenuButton;

	CFont _titleFont;
	CFont _gameCodeFont;
};

