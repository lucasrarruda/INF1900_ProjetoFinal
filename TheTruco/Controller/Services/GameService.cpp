#include "pch.h"
#include <Services/GameService.h>
#include <Serialize.h>
#include <Helpers/Constants/Constants.h>
#include <Helpers/Enums/ModeGame.h>
#include <Helpers/Enums/TrucoEnum.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <thread>
#include <Communication/StructMessage.h>
#include <Exceptions/GameInvalid.h>

using namespace Service;
using namespace Model;
using namespace Helpers;
using namespace Repository::DTOs;
using namespace std;
using namespace Helpers::Constants;
using namespace Helpers::Enums;
using namespace Communication;

void GameService::MonitoringPartnerConnection(shared_future<bool> isPartnerConnected, 
    shared_ptr<GameModel>& currentGame)
{
    thread connectionThread([this, isPartnerConnected, &currentGame]()
        {
            if (isPartnerConnected.get())
            {
                bool isPlayerHost = currentGame->IsHostPlayer(_userModel->GetNickName());

                if (isPlayerHost)
                    ConnectGameAsHost(currentGame, _userModel);
                else
                    ConnectGameAsClient(currentGame, _userModel);
            }
        });
    connectionThread.detach();
}

vector<shared_ptr<GameModel>> GameService::GetAllGames()
{
    vector<GameDTO> gamesDTO = _gameRepository.GetAll(DATABASE_GAMES);

    vector<shared_ptr<GameModel>> games;
    int count = 0;
    for (auto& game : gamesDTO)
    {
        games.insert(games.begin() + count, ToGameModel(game));
        count++;
    }

    return games;
}

shared_ptr<GameModel> GameService::GetGameById(const string& id)
{
    GameDTO game = _gameRepository.GetById(DATABASE_GAMES, id);

    return ToGameModel(game);
}

shared_ptr<GameModel> GameService::SaveGame(shared_ptr<GameModel> game)
{
    GameDTO gameNew;

    gameNew = ToGameDTO(
        make_shared<GameModel>(
            game->GetId(), 
            game->GetPlayGame(), 
            game->GetTurnPlayer(), 
            game->GetModeGame(), 
            game->GetGameCardDeck(), 
            game->GetPlayers(), 
            game->GetHandPoints(),
            game->GetFirstRound(), 
            game->GetPlayerOneDiscardCardKey(), 
            game->GetPlayerTwoDiscardCardKey(), 
            game->GetPlayerThreeDiscardCardKey(), 
            game->GetPlayerFourDiscardCardKey()));
    return ToGameModel(_gameRepository.Save(DATABASE_GAMES, "", gameNew));
}

void GameService::UpdateGame(shared_ptr<GameModel> game)
{
    GameDTO gameNew;

    if (game->GetId().compare("") != 0)
    {
        gameNew = _gameRepository.GetById(DATABASE_GAMES, game->GetId());
    }

    if (Serialize::ConvertGUIDToString(gameNew.Id).compare("") != 0)
    {
        gameNew.PlayGame = game->GetPlayGame();
        gameNew.TurnPlayer = game->GetTurnPlayer();
        gameNew.ModeGame = game->GetModeGame();
        gameNew.GameCardDeck = game->GetGameCardDeck();
        gameNew.Players = game->GetPlayers();

        _gameRepository.Update(DATABASE_GAMES, Serialize::ConvertGUIDToString(gameNew.Id), gameNew);
    }
}

void GameService::UpdateOtherPlayers(shared_ptr<GameModel>& game)
{
    StructMessage message;
    message.MessageSuccessfuly = true;
    message.Content = Serialize::ConvertGameModelToString(game);
 
    if (game->IsHostPlayer(_userModel->GetNickName()))
        _communicationService->SendMessageAsHost(message);
    else
        _communicationService->SendMessageAsClient(message);

    _waitingThread = thread([this, &game]()
        {
            WaitTurn(game);
        });
    //waitingThread.detach();
}

void GameService::RemoveGame(shared_ptr<GameModel> game)
{
    auto gameByid = _gameRepository.GetById(DATABASE_GAMES, game->GetId());

    if (Serialize::ConvertGUIDToString(gameByid.Id).compare("") != 0)
    {
        _gameRepository.Remove(DATABASE_GAMES, Serialize::ConvertGUIDToString(gameByid.Id));
    }
}

shared_ptr<GameModel> GameService::GetConflictingGame(shared_ptr<GameModel> game)
{
    GameDTO gameConflictngDTO = _gameRepository.GetConflictingGame(ToGameDTO(game));

    if (Serialize::ConvertGUIDToString(gameConflictngDTO.Id).compare("") == 0)
        return make_shared<GameModel>();

    shared_ptr<GameModel> gameConflictng = ToGameModel(gameConflictngDTO);

    return gameConflictng;
}

shared_ptr<GameModel> GameService::NewGame(shared_ptr<PlayerModel>& playerHost)
{
    playerHost->SetReady(true);
    playerHost->SetNumberPlayer(1);
    playerHost->SetHostPlayer(true);
    playerHost->SetIsBot(false);

    auto bot1 = make_shared<PlayerModel>("Bot1", true, 2, false);
    bot1->SetIsBot(true);
    auto bot2 = make_shared<PlayerModel>("Bot2", true, 4, false);
    bot2->SetIsBot(true);

    auto gameModel = make_shared<GameModel>("{ChaveAleatorio}", ModeGameEnum::PAIR);
    
    //srand(time(0));
    //int turnPlayer = rand() % 3;
    gameModel->SetTurnPlayer(/*turnPlayer +*/ 1);
    gameModel->SetPlayGame(false);

    gameModel->AddPlayerToGame(playerHost);
    gameModel->AddPlayerToGame(bot1);
    gameModel->AddPlayerToGame(bot2);

    gameModel = SaveGame(gameModel);

    return gameModel;
}

shared_ptr<GameModel> GameService::JoinGame(const string& id, shared_ptr<PlayerModel>& player)
{
    auto game = GetGameById(id);

    player->SetReady(true);
    player->SetNumberPlayer(3);
    player->SetHostPlayer(false);
    player->SetIsBot(false);

    game->AddPlayerToGame(player);

    UpdateGame(game);

    return game;
}

shared_ptr<GameModel> GameService::RecoverLastGame(const string& currentGameID)
{
    auto game = GetGameById(currentGameID);

    UpdateGame(game);

    return game;
}

void GameService::ReadyGame(shared_ptr<GameModel>& currentGame, const string& nickName)
{
    bool allPlayersReady = true;
    for (auto& player : currentGame->GetPlayers())
    {
        if (player.second->GetNickName().compare(nickName) == 0)
        {
            player.second->SetReady(true);
        }

        if (player.second->GetReady() == false)
        {
            allPlayersReady = false;
        }
    }

    if (allPlayersReady)
    { 
        currentGame->SetPlayGame(true);
    }

    UpdateGame(currentGame);
}

void GameService::StartGame(shared_ptr<GameModel>& currentGame)
{
    for (auto& player : currentGame->GetPlayers())
    {
        player.second->SetHandPoints(0);
        player.second->SetRoundPoints(0);
        player.second->GetCardDeck().clear();
    }

    UpdateGame(currentGame);
}

void GameService::LeaveGame(shared_ptr<GameModel>& currentGame, const string& nickName)
{
    for (auto& player : currentGame->GetPlayers())
    {
        if (player.second->GetNickName().compare(nickName) == 0)
        {
            currentGame->RemovePlayerFromGame(player.first);
            break;
        }
    }

    UpdateGame(currentGame);
}

void GameService::SurrenderGame(shared_ptr<GameModel>& currentGame)
{
    RemoveGame(currentGame);
}

void GameService::ResetGame(std::shared_ptr<GameModel>& currentGame)
{
    currentGame->SetId("");
    currentGame->SetPlayGame(false);
    currentGame->SetTurnPlayer(1);
    currentGame->SetModeGame(Helpers::Enums::ModeGameEnum::PAIR);
    currentGame->SetHandPoints(1);
    currentGame->SetFirstRound(true);
    currentGame->SetPlayerOneDiscardCardKey(0);
    currentGame->SetPlayerTwoDiscardCardKey(0);
    currentGame->SetPlayerThreeDiscardCardKey(0);
    currentGame->SetPlayerFourDiscardCardKey(0);
    currentGame->SetGameCardDeck(std::make_shared<Model::CardDeckModel>());
    
    std::map<int, std::shared_ptr<PlayerModel>> players;
    currentGame->SetPlayers(players);
}

void GameService::Hand(std::shared_ptr<GameModel>& currentGame)
{
    currentGame->SetFirstRound(true);
    currentGame->SetHandPoints(static_cast<int>(TrucoEnum::TRUCO));
    currentGame->GetGameCardDeck()->ResetCardDeck();
    
    DistributeCards(currentGame);
    TurnCard(currentGame);

    UpdateGame(currentGame);

    _currentPlayerModel->CopyFrom(currentGame->GetCurrentPlayer(_userModel->GetNickName()));
}

void GameService::PlayCard(shared_ptr<GameModel>& currentGame, const string& nickName, const int& cardKey)
{
    for (auto& player : currentGame->GetPlayers())
    {
        if (player.second->GetNickName().compare(nickName) == 0)
        {
            switch (player.second->GetNumberPlayer())
            {
                case 1:
                    currentGame->SetPlayerOneDiscardCardKey(cardKey);
                    break;
                case 2:
                    currentGame->SetPlayerTwoDiscardCardKey(cardKey);
                    break;
                case 3:
                    currentGame->SetPlayerThreeDiscardCardKey(cardKey);
                    break;
                case 4:
                    currentGame->SetPlayerFourDiscardCardKey(cardKey);
                    break;
            }
        }
    }

    if (currentGame->GetTurnPlayer() == currentGame->GetPlayers().size())
    {
        currentGame->SetTurnPlayer(1);
    }
    else
    {
        currentGame->SetTurnPlayer(currentGame->GetTurnPlayer() + 1);
    }

    UpdateGame(currentGame);
}

void GameService::HideCard(shared_ptr<GameModel>& currentGame, const string& nickName, const int& cardKey)
{
    for (auto& player : currentGame->GetPlayers())
    {
        if (player.second->GetNickName().compare(nickName) == 0)
        {
            player.second->GetCardDeck()[cardKey]->SetReferenceValueActual(0);
            switch (player.second->GetNumberPlayer())
            {
                case 1:
                    currentGame->SetPlayerOneDiscardCardKey(cardKey);
                    break;
                case 2:
                    currentGame->SetPlayerTwoDiscardCardKey(cardKey);
                    break;
                case 3:
                    currentGame->SetPlayerThreeDiscardCardKey(cardKey);
                    break;
                case 4:
                    currentGame->SetPlayerFourDiscardCardKey(cardKey);
                    break;
            }
        }
    }

    if (currentGame->GetTurnPlayer() == currentGame->GetPlayers().size())
    {
        currentGame->SetTurnPlayer(1);
    }
    else
    {
        currentGame->SetTurnPlayer(currentGame->GetTurnPlayer() + 1);
    }

    UpdateGame(currentGame);
}

void GameService::Truco(shared_ptr<GameModel>& currentGame)
{
    switch (currentGame->GetHandPoints())
    {
        case static_cast<int>(TrucoEnum::NORMAL):
            currentGame->SetHandPoints(static_cast<int>(TrucoEnum::TRUCO));
            break;
        case static_cast<int>(TrucoEnum::TRUCO):
            currentGame->SetHandPoints(static_cast<int>(TrucoEnum::SIX));
            break;
        case static_cast<int>(TrucoEnum::SIX):
            currentGame->SetHandPoints(static_cast<int>(TrucoEnum::NINE));
            break;
        case static_cast<int>(TrucoEnum::NINE):
            currentGame->SetHandPoints(static_cast<int>(TrucoEnum::TWELVE));
            break;
    }

    UpdateGame(currentGame);
}

void GameService::FinishedRound(shared_ptr<GameModel>& currentGame)
{
    currentGame->SetFirstRound(false);

    int playerKey = -1;
    int highestValue = 0;
    int valueCard = 0;

    for (auto& player : currentGame->GetPlayers())
    {
        int valueCard = 0;
        switch (player.second->GetNumberPlayer())
        {
            case 1:
                valueCard = player.second->GetCardDeck()[currentGame->GetPlayerOneDiscardCardKey()]->GetReferenceValueActual();
            case 2:
                valueCard = player.second->GetCardDeck()[currentGame->GetPlayerTwoDiscardCardKey()]->GetReferenceValueActual();
            case 3:
                valueCard = player.second->GetCardDeck()[currentGame->GetPlayerThreeDiscardCardKey()]->GetReferenceValueActual();
            case 4:
                valueCard = player.second->GetCardDeck()[currentGame->GetPlayerFourDiscardCardKey()]->GetReferenceValueActual();
        }

        if (valueCard > highestValue)
        {
            highestValue = valueCard;
            playerKey = player.first;
        }
        else if (valueCard == highestValue)
        {
            highestValue = valueCard;
            playerKey = -1;
        }
    }

    for (auto& player : currentGame->GetPlayers())
    {
        switch (player.second->GetNumberPlayer())
        {
            case 1:
                player.second->RemoveCardFromCardDeckPlayer(currentGame->GetPlayerOneDiscardCardKey());
            case 2:
                player.second->RemoveCardFromCardDeckPlayer(currentGame->GetPlayerTwoDiscardCardKey());
            case 3:
                player.second->RemoveCardFromCardDeckPlayer(currentGame->GetPlayerThreeDiscardCardKey());
            case 4:
                player.second->RemoveCardFromCardDeckPlayer(currentGame->GetPlayerFourDiscardCardKey());
        }
    }

    if (playerKey != -1)
    {
        currentGame->GetPlayers()[playerKey]->SetRoundPoints(currentGame->GetPlayers()[playerKey]->GetRoundPoints()+1);
    }

    UpdateGame(currentGame);
}

void GameService::FinishedHand(shared_ptr<GameModel>& currentGame)
{
    bool draw = false;
    bool justOnePlayerWinWithOutDraw = false;
    int playerKey = -1;
    for (auto& player : currentGame->GetPlayers())
    {   
        if (player.second->GetRoundPoints() == 2)
        {
            player.second->SetHandPoints(currentGame->GetHandPoints());
            justOnePlayerWinWithOutDraw = false;
            break;
        }
        else if (player.second->GetRoundPoints() == 1)
        {
            playerKey = player.first;

            if (!draw)
            {
                justOnePlayerWinWithOutDraw = true;
            }
            else
            {
                justOnePlayerWinWithOutDraw = false;
            }
            draw = true;
        }
    }

    if (justOnePlayerWinWithOutDraw)
    {
        currentGame->GetPlayers()[playerKey]->SetHandPoints(currentGame->GetHandPoints());
    }

    UpdateGame(currentGame);
}

bool GameService::ElevenHand(shared_ptr<GameModel>& currentGame, const string& nickName)
{
    bool elevenHand = false;
    for (auto& player : currentGame->GetPlayers())
    {
        if (player.second->GetNickName().compare(nickName) == 0)
        {
            switch (player.second->GetNumberPlayer())
            {
                case 1:
                    elevenHand = ElevenHand(player.second->GetHandPoints() + currentGame->GetPlayers()[3]->GetHandPoints());
                    break;
                case 2:
                    elevenHand = ElevenHand(player.second->GetHandPoints() + currentGame->GetPlayers()[4]->GetHandPoints());
                    break;
                case 3:
                    elevenHand = ElevenHand(player.second->GetHandPoints() + currentGame->GetPlayers()[1]->GetHandPoints());
                    break;
                case 4:
                    elevenHand = ElevenHand(player.second->GetHandPoints() + currentGame->GetPlayers()[2]->GetHandPoints());
                    break;
            }
        }
    }

    return elevenHand;
}

void GameService::RunGame(shared_ptr<GameModel>& currentGame, const string& nickName)
{
    for (auto& player : currentGame->GetPlayers())
    {
        if (player.second->GetNickName().compare(nickName) == 0)
        {
            switch (player.second->GetNumberPlayer())
            {
            case 1:
                currentGame->GetPlayers()[2]->SetHandPoints(1);
                break;
            case 2:
                currentGame->GetPlayers()[3]->SetHandPoints(1);
                break;
            case 3:
                currentGame->GetPlayers()[4]->SetHandPoints(1);
                break;
            case 4:
                currentGame->GetPlayers()[1]->SetHandPoints(1);
                break;
            }
        }
    }

    UpdateGame(currentGame);
}

bool GameService::IronHand(shared_ptr<GameModel>& currentGame)
{
    bool ironHand = true;
    for (auto& player : currentGame->GetPlayers())
    {
        if (!ElevenHand(currentGame, player.second->GetNickName()))
        {
            ironHand = false;
            break;
        }
    }

    return ironHand;
}

GameDTO GameService::ToGameDTO(std::shared_ptr<GameModel> gameModel)
{
    return GameDTO(
        gameModel->GetPlayGame(), 
        gameModel->GetTurnPlayer(), 
        gameModel->GetModeGame(), 
        gameModel->GetGameCardDeck(), 
        gameModel->GetPlayers(), 
        gameModel->GetHandPoints(),
        gameModel->GetFirstRound(), 
        gameModel->GetPlayerOneDiscardCardKey(), 
        gameModel->GetPlayerTwoDiscardCardKey(), 
        gameModel->GetPlayerThreeDiscardCardKey(), 
        gameModel->GetPlayerFourDiscardCardKey());
}

shared_ptr<GameModel> GameService::ToGameModel(Repository::DTOs::GameDTO gameDTO)
{
    return make_shared<GameModel>(
        Serialize::ConvertGUIDToString(gameDTO.Id), 
        gameDTO.PlayGame, 
        gameDTO.TurnPlayer, 
        gameDTO.ModeGame, 
        gameDTO.GameCardDeck, 
        gameDTO.Players, 
        gameDTO.HandPoints, 
        gameDTO.FirstRound, 
        gameDTO.PlayerOneDiscardCardKey, 
        gameDTO.PlayerTwoDiscardCardKey, 
        gameDTO.PlayerThreeDiscardCardKey, 
        gameDTO.PlayerFourDiscardCardKey);
}

void GameService::DistributeCards(shared_ptr<GameModel>& currentGame)
{
    vector<int> cardsRemoved;
    int cardsDealt = 0;
    int countCards = 0;
    for (auto& player : currentGame->GetPlayers())
    {
        cardsDealt = 0;
        for (auto& card : currentGame->GetGameCardDeck()->GetCardDeck())
        {
            if (cardsDealt < 3)
            {
                if (std::find(cardsRemoved.begin(), cardsRemoved.end(), card.first) == cardsRemoved.end())
                {
                    player.second->AddCardToCardDeckPlayer(card.second);
                    cardsRemoved.insert(cardsRemoved.begin() + countCards, card.first);
                    cardsDealt++;
                    countCards++;
                }
            }
        }
    }

    for (auto& cardRemoved : cardsRemoved)
    {
        currentGame->GetGameCardDeck()->RemoveCardFromCardDeck(cardRemoved);
    }
}

void GameService::TurnCard(shared_ptr<GameModel>& currentGame)
{
    int turnCardKey = 0;
    int manila;
    for (auto& card : currentGame->GetGameCardDeck()->GetCardDeck())
    {
        turnCardKey = card.first;
        if (card.second->GetReferenceValue() == static_cast<int>(CardsValueEnum::THREE))
        {
            manila = static_cast<int>(CardsValueEnum::FOUR);
        }
        else
        {
            manila = card.first + 1;
        }
        break;
    }

    currentGame->GetGameCardDeck()->RemoveCardFromCardDeck(turnCardKey);


    // Acho que esse trecho de c�digo n�o � necess�rio, verificar na valida��o ------
    vector<int> manilas;
    int countManilas = 0;
    for (auto& card : currentGame->GetGameCardDeck()->GetCardDeck())
    {
        if (card.second->GetReferenceValue() == manila)
        {
            manilas.insert(manilas.begin() + countManilas, card.first);
            countManilas++;
        }
    }

    for (auto& manila : manilas)
    {
        currentGame->GetGameCardDeck()->SetCardManila(manila);
    }
    // -----------------------------------------------------

    for (auto& player : currentGame->GetPlayers())
    {
        for (auto& card : player.second->GetCardDeck())
        {
            if (card.second->GetReferenceValue() == manila)
            {
                card.second->SetReferenceValueActual(true);
            }
        }
    }
}

bool GameService::ElevenHand(const int& totalPoints)
{
    if (totalPoints == 11)
    {
        return true;
    }

    return false;
}

void GameService::ConnectGameAsHost(shared_ptr<GameModel>& currentGame, shared_ptr<UserModel>& currentUser)
{
    StructMessage receivedConfirmation = _communicationService->ReceiveMessageHost();
    if (!receivedConfirmation.MessageSuccessfuly)
    {
        throw Exceptions::GameInvalid("O jogador no conseguiu se conectar na partida!");
    }

    auto clientGameModel = Serialize::ConvertStringToGameModel(receivedConfirmation.Content);
    
    currentGame->AddPlayerToGame(clientGameModel->GetPlayers()[3]);

    StartGame(currentGame);
    Hand(currentGame);

    StructMessage gameMessage;
    gameMessage.MessageSuccessfuly = true;
    gameMessage.Content = Serialize::ConvertGameModelToString(currentGame);
    _communicationService->SendMessageAsHost(gameMessage);
}

void GameService::ConnectGameAsClient(shared_ptr<GameModel>& currentGame, shared_ptr<UserModel>& currentUser)
{
    StructMessage message;
    message.MessageSuccessfuly = true;
    message.Content = Serialize::ConvertGameModelToString(currentGame);
    bool messageSent = _communicationService->SendMessageAsClient(message);

    if (!messageSent)
    {
        throw Exceptions::GameInvalid("O host nao conseguiu inicializar a partida!");
    }

    StructMessage receivedGameMessage;
    receivedGameMessage = _communicationService->ReceiveMessageClient();
    if (receivedGameMessage.MessageSuccessfuly)
    {
        currentGame->CopyFrom(Serialize::ConvertStringToGameModel(receivedGameMessage.Content));
        _currentPlayerModel->CopyFrom(currentGame->GetCurrentPlayer(_userModel->GetNickName()));
    }

    WaitTurn(currentGame);
}

void GameService::WaitTurn(shared_ptr<GameModel>& currentGame)
{
    bool isPlayerHost = currentGame->IsHostPlayer(_userModel->GetNickName());

    StructMessage receivedMessage;
    if (isPlayerHost)
    {
        receivedMessage = _communicationService->ReceiveMessageHost();
        // TODO: resolver jogada
        currentGame->CopyFrom(Serialize::ConvertStringToGameModel(receivedMessage.Content));
        _currentPlayerModel->CopyFrom(currentGame->GetCurrentPlayer(_userModel->GetNickName()));

        if (currentGame->GetLeaveGame())
        {
            currentGame->SwitchTurn();
            RemoveGame(currentGame);
        }
    }
    else
    {
        receivedMessage = _communicationService->ReceiveMessageClient();
        // TODO: resolver jogada
        currentGame->CopyFrom(Serialize::ConvertStringToGameModel(receivedMessage.Content));
        _currentPlayerModel->CopyFrom(currentGame->GetCurrentPlayer(_userModel->GetNickName()));

        if (currentGame->GetLeaveGame())
        {
            currentGame->SwitchTurn();
            RemoveGame(currentGame);
        }
    }
}
