#pragma once
#include <ViewBase.h>
class NewGameView : public ViewBase, public CFrameWnd
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
	CStatic _GameCode;
	CStatic _labelPlayerOne;
	CStatic _labelPlayerTwo;
	CStatic _labelPlayerThree;
	CStatic _labelPlayerFour;
	CButton _startGameButton;
	CButton _backMenuButton;

	CFont _titleFont;
	CFont _gameCodeFont;
};

