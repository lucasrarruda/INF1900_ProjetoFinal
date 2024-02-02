#pragma once
#include <Interfaces/ViewBase.h>

class NewGameView : public Interfaces::ViewBase, public CFrameWnd
{
public:
	NewGameView(CWnd* parentWindow);
	~NewGameView() = default;

	void Create();
	void Updated();
	void Show();
	void Hide();

private:
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

