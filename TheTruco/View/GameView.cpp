#include "pch.h"
#include <GameView.h>
#include <DisplayHelper.h>
#include <GeneralHelper.h>

using namespace std;

GameView::GameView(CWnd* parentWindow, std::shared_ptr<Controller::GameController> gameController) :
	Interfaces::ViewBase(), 
	_parentWindow(parentWindow),
	_gameController(gameController)
{
	_userModel = _gameController->GetUserModel();
	_currentPlayerModel = _gameController->GetCurrentPlayerModel();
}

void GameView::Create()
{
	_userModel->Attach(shared_from_this());
	_currentPlayerModel->Attach(shared_from_this());

	CreateGameControls();
	CreateGameScore();
	CreateGameCards();
}

void GameView::Update()
{
	CString gameCode = GeneralHelper::StringToCString(_userModel->GetCurrentGameID());
	_gameCode.SetWindowTextW(gameCode);
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
	_gameConsole.ShowWindow(SW_SHOW);
	_leaveGameButton.ShowWindow(SW_SHOW);
	_yourCardCoverOne.ShowWindow(SW_SHOW);
	_yourCardCoverTwo.ShowWindow(SW_SHOW);
	_yourCardCoverThree.ShowWindow(SW_SHOW);
	_labelGameCode.ShowWindow(SW_SHOW);
	_gameCode.ShowWindow(SW_SHOW);
	_copyToClipboardButton.ShowWindow(SW_SHOW);
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
	_gameConsole.ShowWindow(SW_HIDE);
	_leaveGameButton.ShowWindow(SW_HIDE);
	_yourCardCoverOne.ShowWindow(SW_HIDE);
	_yourCardCoverTwo.ShowWindow(SW_HIDE);
	_yourCardCoverThree.ShowWindow(SW_HIDE);
	_labelGameCode.ShowWindow(SW_HIDE);
	_gameCode.ShowWindow(SW_HIDE);
	_copyToClipboardButton.ShowWindow(SW_HIDE);
}

void GameView::CopyGameCodeToClipboard()
{
	CString gameCode;
	_gameCode.GetWindowTextW(gameCode);
	_gameController->CopyGameCodetoClipboard(GeneralHelper::CStringToWstring(gameCode));
}

void GameView::DropCardOne()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_gameController->PlayCard(1);
}

void GameView::DropCardTwo()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_gameController->PlayCard(2);
}

void GameView::DropCardThree()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_gameController->PlayCard(3);
}

void GameView::DropCoveredCardOne()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_gameController->PlayCoveredCard(1);
}

void GameView::DropCoveredCardTwo()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_gameController->PlayCoveredCard(2);
}

void GameView::DropCoveredCardThree()
{
	_yourDroppedCard.ShowWindow(SW_SHOW);
	_gameController->PlayCoveredCard(3);
}

void GameView::NotifyTruco()
{
	_gameConsole.SetWindowTextW(_T("Truco!"));
	_gameController->NotifyTruco();
}

void GameView::LeaveGame()
{
	_gameController->LeaveGame();
}

void GameView::CreateGameControls()
{
	auto [dpiX, dpiY] = DisplayHelper::GetMonitorDpi();

	_labelFont.CreatePointFont(static_cast<int>(120 * dpiX), _T("Arial"));

	CRect labelGameCodeRect
	{
		static_cast<int>(20 * dpiX),
		static_cast<int>(880 * dpiY),
		static_cast<int>(340 * dpiX),
		static_cast<int>(900 * dpiY)
	};
	_labelGameCode.Create(_T("Game Code: "), WS_CHILD | WS_VISIBLE | SS_LEFT | BS_CENTER, labelGameCodeRect, _parentWindow);
	_labelGameCode.SetFont(&_labelFont);

	CRect gameCodeRect
	{
		static_cast<int>(20 * dpiX),
		static_cast<int>(905 * dpiY),
		static_cast<int>(340 * dpiX),
		static_cast<int>(925 * dpiY)
	};
	_gameCode.Create(_T("{00000000-0000-0000-0000-000000000000}"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, gameCodeRect, _parentWindow);
	_gameCode.SetFont(&_labelFont);

	CRect copyGameCodeButtonRect
	{
		static_cast<int>(20 * dpiX),
		static_cast<int>(930 * dpiY),
		static_cast<int>(340 * dpiX),
		static_cast<int>(960 * dpiY)
	};
	_copyToClipboardButton.Create(_T("Copy To Clipboard"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, copyGameCodeButtonRect, _parentWindow, IDC_COPY_TO_CLIPBOARD_BUTTON);
	_copyToClipboardButton.SetFont(&_labelFont);

	CRect gameConsoleRect
	{
		static_cast<int>(20 * dpiX),
		static_cast<int>(970 * dpiY),
		static_cast<int>(340 * dpiX),
		static_cast<int>(1090 * dpiY)
	};
	_gameConsole.Create(_T("output messages goes here..."), WS_CHILD | WS_VISIBLE | SS_LEFT, gameConsoleRect, _parentWindow);
	_gameConsole.SetFont(&_labelFont);

	_trucoButtonFont.CreatePointFont(150, _T("Arial"));
	CRect trucoButtonRect
	{
		static_cast<int>(950 * dpiX),
		static_cast<int>(970 * dpiY),
		static_cast<int>(1100 * dpiX),
		static_cast<int>(1080 * dpiY)
	};
	_trucoButton.Create(_T("Truco!"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, trucoButtonRect, _parentWindow, IDC_TRUCO_BUTTON);
	_trucoButton.SetFont(&_trucoButtonFont);

	CRect leaveGameButtonRect
	{
		static_cast<int>(950 * dpiX),
		static_cast<int>(20 * dpiY),
		static_cast<int>(1100 * dpiX),
		static_cast<int>(50 * dpiY)
	};
	_leaveGameButton.Create(_T("Leave Game"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, leaveGameButtonRect, _parentWindow, IDC_LEAVE_GAME_BUTTON);
	_leaveGameButton.SetFont(&_labelFont);
}

void GameView::CreateGameScore()
{
	auto [dpiX, dpiY] = DisplayHelper::GetMonitorDpi();
	_scoresFont.CreatePointFont(static_cast<int>(90 * dpiX), _T("Arial"));

	CRect labelGameScoreRect
	{
		static_cast<int>(80 * dpiX),
		static_cast<int>(20 * dpiY),
		static_cast<int>(190 * dpiX),
		static_cast<int>(45 * dpiY)
	};
	_labelGameScore.Create(_T("Game Score:"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, labelGameScoreRect, _parentWindow);
	_labelGameScore.SetFont(&_scoresFont);
	
	CRect labelGamePointsRect
	{
		static_cast<int>(200 * dpiX),
		static_cast<int>(20 * dpiY),
		static_cast<int>(260 * dpiX),
		static_cast<int>(45 * dpiY)
	};
	_labelGamePoints.Create(_T("Points:"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, labelGamePointsRect, _parentWindow);
	_labelGamePoints.SetFont(&_scoresFont);

	CRect labelYourRect
	{
		static_cast<int>(20 * dpiX),
		static_cast<int>(50 * dpiY),
		static_cast<int>(70 * dpiX),
		static_cast<int>(70 * dpiY)
	};
	_labelYour.Create(_T("Your:"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, labelYourRect, _parentWindow);
	_labelYour.SetFont(&_scoresFont);
	
	CRect labelTheirRect
	{
		static_cast<int>(20 * dpiX),
		static_cast<int>(80 * dpiY),
		static_cast<int>(70 * dpiX),
		static_cast<int>(100 * dpiY)
	};
	_labelTheir.Create(_T("Their:"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, labelTheirRect, _parentWindow);
	_labelTheir.SetFont(&_scoresFont);

	CRect yourScoreRect
	{
		static_cast<int>(125 * dpiX),
		static_cast<int>(50 * dpiY),
		static_cast<int>(155 * dpiX),
		static_cast<int>(70 * dpiY)
	};
	_yourScore.Create(_T("2"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, yourScoreRect, _parentWindow);
	_yourScore.SetFont(&_scoresFont);
	
	CRect theirScoreRect
	{
		static_cast<int>(125 * dpiX),
		static_cast<int>(80 * dpiY),
		static_cast<int>(155 * dpiX),
		static_cast<int>(100 * dpiY)
	};
	_theirScore.Create(_T("1"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, theirScoreRect, _parentWindow);
	_theirScore.SetFont(&_scoresFont);
	
	CRect yourPointsRect
	{
		static_cast<int>(210 * dpiX),
		static_cast<int>(50 * dpiY),
		static_cast<int>(240 * dpiX),
		static_cast<int>(70 * dpiY)
	};
	_yourPoints.Create(_T("5"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, yourPointsRect, _parentWindow);
	_yourPoints.SetFont(&_scoresFont);
	
	CRect theirPointsRect
	{
		static_cast<int>(210 * dpiX),
		static_cast<int>(80 * dpiY),
		static_cast<int>(240 * dpiX),
		static_cast<int>(100 * dpiY)
	};
	_theirPoints.Create(_T("6"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, theirPointsRect, _parentWindow);
	_theirPoints.SetFont(&_scoresFont);
}

void GameView::CreateGameCards()
{
	auto [dpiX, dpiY] = DisplayHelper::GetMonitorDpi();
	_buttonFont.CreatePointFont(static_cast<int>(130 * dpiX), _T("Arial"));

	CRect cardsDeckRect
	{
		static_cast<int>(520 * dpiX),
		static_cast<int>(430 * dpiY),
		static_cast<int>(650 * dpiX),
		static_cast<int>(610 * dpiY)
	};
	_cardsDeck.Create(_T("Cards Deck"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, cardsDeckRect, _parentWindow);
	_cardsDeck.SetFont(&_buttonFont);

	CRect cardsPlayerPartnerRect
	{
		static_cast<int>(480 * dpiX),
		static_cast<int>(30 * dpiY),
		static_cast<int>(680 * dpiX),
		static_cast<int>(210 * dpiY)
	};
	_cardsPlayerPartner.Create(_T("Partner Cards(3)"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, cardsPlayerPartnerRect, _parentWindow);
	_cardsPlayerPartner.SetFont(&_buttonFont);
	
	CRect cardsOpponentLeftRect
	{
		static_cast<int>(30 * dpiX),
		static_cast<int>(430 * dpiY),
		static_cast<int>(230 * dpiX),
		static_cast<int>(610 * dpiY)
	};
	_cardsOpponentLeft.Create(_T("Opponent Cards (3)"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, cardsOpponentLeftRect, _parentWindow);
	_cardsOpponentLeft.SetFont(&_buttonFont);
	
	CRect cardsOpponentRightRect
	{
		static_cast<int>(900 * dpiX),
		static_cast<int>(430 * dpiY),
		static_cast<int>(1100 * dpiX),
		static_cast<int>(610 * dpiY)
	};
	_cardsOpponentRight.Create(_T("Opponent Cards (3)"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, cardsOpponentRightRect, _parentWindow);
	_cardsOpponentRight.SetFont(&_buttonFont);

	CRect yourCardOneRect
	{
		static_cast<int>(370 * dpiX),
		static_cast<int>(840 * dpiY),
		static_cast<int>(500 * dpiX),
		static_cast<int>(1020 * dpiY)
	};
	_yourCardOne.Create(_T("Card One"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, yourCardOneRect, _parentWindow, IDC_YOUR_CARD_ONE_BUTTON);
	_yourCardOne.SetFont(&_buttonFont);
	
	CRect yourCardTwoRect
	{
		static_cast<int>(510 * dpiX),
		static_cast<int>(840 * dpiY),
		static_cast<int>(640 * dpiX),
		static_cast<int>(1020 * dpiY)
	};
	_yourCardTwo.Create(_T("Card Two"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, yourCardTwoRect, _parentWindow, IDC_YOUR_CARD_TWO_BUTTON);
	_yourCardTwo.SetFont(&_buttonFont);
	
	CRect yourCardThreeRect
	{
		static_cast<int>(650 * dpiX),
		static_cast<int>(840 * dpiY),
		static_cast<int>(780 * dpiX),
		static_cast<int>(1020 * dpiY)
	};
	_yourCardThree.Create(_T("Card Three"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, yourCardThreeRect, _parentWindow, IDC_YOUR_CARD_THREE_BUTTON);
	_yourCardThree.SetFont(&_buttonFont);

	CRect yourCardCoverOneRect
	{
		static_cast<int>(370 * dpiX),
		static_cast<int>(1030 * dpiY),
		static_cast<int>(500 * dpiX),
		static_cast<int>(1090 * dpiY)
	};
	_yourCardCoverOne.Create(_T("Card One"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, yourCardCoverOneRect, _parentWindow, IDC_YOUR_CARD_COVER_ONE_BUTTON);
	_yourCardCoverOne.SetFont(&_buttonFont);

	CRect yourCardCoverTwoRect
	{
		static_cast<int>(510 * dpiX),
		static_cast<int>(1030 * dpiY),
		static_cast<int>(640 * dpiX),
		static_cast<int>(1090 * dpiY)
	};
	_yourCardCoverTwo.Create(_T("Card Two"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, yourCardCoverTwoRect, _parentWindow, IDC_YOUR_CARD_COVER_TWO_BUTTON);
	_yourCardCoverTwo.SetFont(&_buttonFont);

	CRect yourCardCoverThreeRect
	{
		static_cast<int>(650 * dpiX),
		static_cast<int>(1030 * dpiY),
		static_cast<int>(780 * dpiX),
		static_cast<int>(1090 * dpiY)
	};
	_yourCardCoverThree.Create(_T("Card Three"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, yourCardCoverThreeRect, _parentWindow, IDC_YOUR_CARD_COVER_THREE_BUTTON);
	_yourCardCoverThree.SetFont(&_buttonFont);

	CRect yourDroppedCardRect
	{
		static_cast<int>(620 * dpiX),
		static_cast<int>(700 * dpiY),
		static_cast<int>(800 * dpiX),
		static_cast<int>(820 * dpiY)
	};
	_yourDroppedCard.Create(_T("Your Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, yourDroppedCardRect, _parentWindow);
	_yourDroppedCard.SetFont(&_buttonFont);
	
	CRect partnerDroppedCardRect
	{
		static_cast<int>(350 * dpiX),
		static_cast<int>(220 * dpiY),
		static_cast<int>(530 * dpiX),
		static_cast<int>(350 * dpiY)
	};
	_partnerDroppedCard.Create(_T("Partner Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, partnerDroppedCardRect, _parentWindow);
	_partnerDroppedCard.SetFont(&_buttonFont);
	
	CRect opponentLeftDroppedCardRect
	{
		static_cast<int>(240 * dpiX),
		static_cast<int>(520 * dpiY),
		static_cast<int>(420 * dpiX),
		static_cast<int>(650 * dpiY)
	};
	_opponentLeftDroppedCard.Create(_T("Op. Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, opponentLeftDroppedCardRect, _parentWindow);
	_opponentLeftDroppedCard.SetFont(&_buttonFont);
	
	CRect opponentRightDroppedCardRect
	{
		static_cast<int>(710 * dpiX),
		static_cast<int>(380 * dpiY),
		static_cast<int>(890 * dpiX),
		static_cast<int>(510 * dpiY)
	};
	_opponentRightDroppedCard.Create(_T("Op. Dropped Card"), WS_CHILD | WS_VISIBLE | SS_CENTER | BS_CENTER, opponentRightDroppedCardRect, _parentWindow);
	_opponentRightDroppedCard.SetFont(&_buttonFont);
}
