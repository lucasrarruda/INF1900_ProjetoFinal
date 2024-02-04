#pragma once
#include <Interfaces/ViewBase.h>
#include <GameController.h>

class GameView : public Interfaces::ViewBase, public CFrameWnd, public std::enable_shared_from_this<GameView>
{
public:
	GameView(CWnd* parentWindow, std::shared_ptr<Controller::GameController> gameController);
	~GameView() = default;

	void Create();
	void Update();
	void Show();
	void Hide();

	void CopyGameCodeToClipboard();
	void DropCardOne();
	void DropCardTwo();
	void DropCardThree();
	void NotifyTruco();
	void LeaveGame();

private:
	std::shared_ptr<Controller::GameController> _gameController;
	std::shared_ptr<Model::UserModel> _userModel;

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

