#include "pch.h"
#include <GameView.h>

using namespace std;

GameView::GameView(CWnd* parentWindow) : Interfaces::ViewBase(), _parentWindow(parentWindow)
{
}

void GameView::Create()
{
	CreateGameScore();
	CreateGameCards();
	
	_labelConsole.Create(_T("Console:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(20, 990, 90, 1010), _parentWindow);
	_gameConsole.Create(_T("output messages goes here..."), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(20, 1020, 300, 1100), _parentWindow);

	_trucoButtonFont.CreatePointFont(150, _T("Arial"));
	_trucoButton.Create(_T("Truco!"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(950, 970, 1100, 1080), _parentWindow, IDC_TRUCO_BUTTON);
	_trucoButton.SetFont(&_trucoButtonFont);	
}

void GameView::Updated()
{
	// Update UI with new model values
}

void GameView::Show()
{
	_labelGameScore.ShowWindow(SW_SHOW);
	_labelGamePoints.ShowWindow(SW_SHOW);
	_labelYour.ShowWindow(SW_SHOW);
	_labelTheir.ShowWindow(SW_SHOW);
	_yourScore.ShowWindow(SW_SHOW);
	_theirScore.ShowWindow(SW_SHOW);
	_yourPoints.ShowWindow(SW_SHOW);
	_theirPoints.ShowWindow(SW_SHOW);
	_trucoButton.ShowWindow(SW_SHOW);
	_cardsDeck.ShowWindow(SW_SHOW);
	_yourCardOne.ShowWindow(SW_SHOW);
	_yourCardTwo.ShowWindow(SW_SHOW);
	_yourCardThree.ShowWindow(SW_SHOW);
	_cardsPlayerPartner.ShowWindow(SW_SHOW);
	_cardsOpponentLeft.ShowWindow(SW_SHOW);
	_cardsOpponentRight.ShowWindow(SW_SHOW);
	_yourDroppedCard.ShowWindow(SW_HIDE);
	_partnerDroppedCard.ShowWindow(SW_HIDE);
	_opponentLeftDroppedCard.ShowWindow(SW_HIDE);
	_opponentRightDroppedCard.ShowWindow(SW_HIDE);
	_labelConsole.ShowWindow(SW_SHOW);
	_gameConsole.ShowWindow(SW_SHOW);
}

void GameView::Hide()
{
	_labelGameScore.ShowWindow(SW_HIDE);
	_labelGamePoints.ShowWindow(SW_HIDE);
	_labelYour.ShowWindow(SW_HIDE);
	_labelTheir.ShowWindow(SW_HIDE);
	_yourScore.ShowWindow(SW_HIDE);
	_theirScore.ShowWindow(SW_HIDE);
	_yourPoints.ShowWindow(SW_HIDE);
	_theirPoints.ShowWindow(SW_HIDE);
	_trucoButton.ShowWindow(SW_HIDE);
	_cardsDeck.ShowWindow(SW_HIDE);
	_yourCardOne.ShowWindow(SW_HIDE);
	_yourCardTwo.ShowWindow(SW_HIDE);
	_yourCardThree.ShowWindow(SW_HIDE);
	_cardsPlayerPartner.ShowWindow(SW_HIDE);
	_cardsOpponentLeft.ShowWindow(SW_HIDE);
	_cardsOpponentRight.ShowWindow(SW_HIDE);
	_yourDroppedCard.ShowWindow(SW_HIDE);
	_partnerDroppedCard.ShowWindow(SW_HIDE);
	_opponentLeftDroppedCard.ShowWindow(SW_HIDE);
	_opponentRightDroppedCard.ShowWindow(SW_HIDE);
	_labelConsole.ShowWindow(SW_HIDE);
	_gameConsole.ShowWindow(SW_HIDE);
}

void GameView::DropCardOne()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_partnerDroppedCard.ShowWindow(SW_SHOW);
}

void GameView::DropCardTwo()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_opponentLeftDroppedCard.ShowWindow(SW_SHOW);
}

void GameView::DropCardThree()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_opponentRightDroppedCard.ShowWindow(SW_SHOW);
}

void GameView::NotifyTruco()
{
	_gameConsole.SetWindowTextW(_T("Truco!"));
}

void GameView::CreateGameScore()
{
	_labelGameScore.Create(_T("Game Score:"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(80, 20, 190, 45), _parentWindow);
	_labelGamePoints.Create(_T("Points:"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(200, 20, 260, 45), _parentWindow);

	_labelYour.Create(_T("Your:"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(20, 50, 70, 70), _parentWindow);
	_labelTheir.Create(_T("Their:"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(20, 80, 70, 100), _parentWindow);

	_yourScore.Create(_T("2"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(125, 50, 155, 70), _parentWindow);
	_theirScore.Create(_T("1"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(125, 80, 155, 100), _parentWindow);
	_yourPoints.Create(_T("5"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(210, 50, 240, 70), _parentWindow);
	_theirPoints.Create(_T("6"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(210, 80, 240, 100), _parentWindow);
}

void GameView::CreateGameCards()
{
	_cardsDeck.Create(_T("Cards Deck"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(520, 430, 650, 610), _parentWindow);

	_cardsPlayerPartner.Create(_T("Partner Cards(3)"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(480, 30, 680, 210), _parentWindow);
	_cardsOpponentLeft.Create(_T("Opponent Cards (3)"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(30, 430, 230, 610), _parentWindow);
	_cardsOpponentRight.Create(_T("Opponent Cards (3)"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(930, 430, 1130, 610), _parentWindow);

	_yourCardOne.Create(_T("Card One"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(370, 920, 500, 1100), _parentWindow, IDC_YOUR_CARD_ONE_BUTTON);
	_yourCardTwo.Create(_T("Card Two"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(510, 920, 640, 1100), _parentWindow, IDC_YOUR_CARD_TWO_BUTTON);
	_yourCardThree.Create(_T("Card Three"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(650, 920, 780, 1100), _parentWindow, IDC_YOUR_CARD_THREE_BUTTON);

	_yourDroppedCard.Create(_T("Your Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(620, 780, 800, 910), _parentWindow);
	_partnerDroppedCard.Create(_T("Partner Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(350, 220, 530, 350), _parentWindow);
	_opponentLeftDroppedCard.Create(_T("Op. Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(240, 520, 420, 650), _parentWindow);
	_opponentRightDroppedCard.Create(_T("Op. Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, CRect(740, 380, 920, 510), _parentWindow);
}
