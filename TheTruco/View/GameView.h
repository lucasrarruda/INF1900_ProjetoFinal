#pragma once
#include <Interfaces/ViewBase.h>

class GameView : public Interfaces::ViewBase, public CFrameWnd
{
public:
	GameView(CWnd* parentWindo/*, shared_ptr<Controller::GameController> gameCotnroller*/);
	~GameView() = default;

	void Create();
	void Update();
	void Show();
	void Hide();

	void DropCardOne();
	void DropCardTwo();
	void DropCardThree();
	void NotifyTruco();

private:
	CWnd* _parentWindow;
	CStatic _labelGameScore;
	CStatic _labelGamePoints;
	CStatic _labelYour;
	CStatic _labelTheir;
	CStatic _yourScore;
	CStatic _yourPoints;
	CStatic _theirScore;
	CStatic _theirPoints;
	CStatic _labelGameCode;
	CStatic _gameCode;
	CStatic _gameConsole;
	CButton _trucoButton;
	CButton _leaveGameButton;
	CButton _copyToClipboardButton;
	CFont _trucoButtonFont;
	CFont _labelFont;
	CFont _scoresFont;
	CFont _buttonFont;

	CStatic _cardsDeck;
	CStatic _yourDroppedCard;
	CStatic _partnerDroppedCard;
	CStatic _opponentLeftDroppedCard;
	CStatic _opponentRightDroppedCard;

	CStatic _cardsPlayerPartner;
	CStatic _cardsOpponentLeft;
	CStatic _cardsOpponentRight;

	CButton _yourCardOne;
	CButton _yourCardTwo;
	CButton _yourCardThree;
	CButton _yourCardCoverOne;
	CButton _yourCardCoverTwo;
	CButton _yourCardCoverThree;

	void CreateGameControls();
	void CreateGameScore();
	void CreateGameCards();
};

