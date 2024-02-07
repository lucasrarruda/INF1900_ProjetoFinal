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

void Service::GameService::MonitoringPartnerConnection(shared_future<bool> isPartnerConnected, 
    std::shared_ptr<Model::GameModel>& currentGame)
{
    _gameThread = thread([this, isPartnerConnected, &currentGame]()
        {
            if (isPartnerConnected.get())
            {
                bool isPlayerHost = currentGame->IsHostPlayer(_userModel->GetNickName());

                if (isPlayerHost)
                    ConnectGameAsHost(currentGame, _userModel);
                else
                    ConnectGameAsClient(currentGame, _userModel);
            }
            else
            {
                // TODO: não conectado
            }
        });
}

vector<std::shared_ptr<Model::GameModel>> GameService::GetAllGames()
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

std::shared_ptr<Model::GameModel> GameService::GetGameById(const string& id)
{
    GameDTO game = _gameRepository.GetById(DATABASE_GAMES, id);

    return ToGameModel(game);
}

std::shared_ptr<Model::GameModel> GameService::SaveGame(std::shared_ptr<Model::GameModel> game)
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

void GameService::UpdateGame(std::shared_ptr<Model::GameModel> game)
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

void Service::GameService::UpdateOtherPlayers(std::shared_ptr<Model::GameModel> game)
{
    StructMessage message;
    message.MessageSuccessfuly = true;
    message.Content = Serialize::ConvertGameModelToString(game);
 
    if (game->IsHostPlayer(_userModel->GetNickName()))
        _communicationService->SendMessageAsHost(message);
    else
        _communicationService->SendMessageAsClient(message);
}

void GameService::RemoveGame(std::shared_ptr<Model::GameModel> game)
{
    auto gameByid = _gameRepository.GetById(DATABASE_GAMES, game->GetId());

    if (Serialize::ConvertGUIDToString(gameByid.Id).compare("") != 0)
    {
        _gameRepository.Remove(DATABASE_GAMES, Serialize::ConvertGUIDToString(gameByid.Id));
    }
}

std::shared_ptr<Model::GameModel> GameService::GetConflictingGame(std::shared_ptr<Model::GameModel> game)
{
    GameDTO gameConflictngDTO = _gameRepository.GetConflictingGame(ToGameDTO(game));

    if (Serialize::ConvertGUIDToString(gameConflictngDTO.Id).compare("") == 0)
        return make_shared<GameModel>();

    shared_ptr<GameModel> gameConflictng = ToGameModel(gameConflictngDTO);

    return gameConflictng;
}

std::shared_ptr<Model::GameModel> GameService::NewGame(shared_ptr<PlayerModel>& playerHost)
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
    
    srand(time(0));
    int turnPlayer = rand() % 3;
    gameModel->SetTurnPlayer(turnPlayer + 1);
    gameModel->SetPlayGame(false);

    gameModel->AddPlayerToGame(playerHost);
    gameModel->AddPlayerToGame(bot1);
    gameModel->AddPlayerToGame(bot2);

    gameModel = SaveGame(gameModel);

    return gameModel;
}

std::shared_ptr<Model::GameModel> GameService::JoinGame(const string& id, shared_ptr<PlayerModel>& player)
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

std::shared_ptr<Model::GameModel> GameService::RecoverLastGame(const string& currentGameID)
{
    auto game = GetGameById(currentGameID);

    UpdateGame(game);

    return game;
}

void GameService::ReadyGame(std::shared_ptr<Model::GameModel>& currentGame, const string& nickName)
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

void GameService::StartGame(std::shared_ptr<Model::GameModel>& currentGame)
{
    for (auto& player : currentGame->GetPlayers())
    {
        player.second->SetHandPoints(0);
        player.second->SetRoundPoints(0);
        player.second->GetCardDeck().clear();
    }

    UpdateGame(currentGame);
}

void GameService::LeaveGame(std::shared_ptr<Model::GameModel>& currentGame, const string& nickName)
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

void GameService::SurrenderGame(std::shared_ptr<Model::GameModel>& currentGame)
{
    RemoveGame(currentGame);
}

void GameService::Hand(std::shared_ptr<Model::GameModel>& currentGame)
{
    currentGame->SetFirstRound(true);
    currentGame->SetHandPoints(static_cast<int>(TrucoEnum::TRUCO));
    currentGame->GetGameCardDeck()->ResetCardDeck();
    
    DistributeCards(currentGame);
    TurnCard(currentGame);

    UpdateGame(currentGame);
}

void GameService::PlayCard(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName, const int& cardKey)
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

void GameService::HideCard(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName, const int& cardKey)
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

void GameService::Truco(std::shared_ptr<Model::GameModel>& currentGame)
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

void GameService::FinishedRound(std::shared_ptr<Model::GameModel>& currentGame)
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

void GameService::FinishedHand(std::shared_ptr<Model::GameModel>& currentGame)
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

bool GameService::ElevenHand(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName)
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

void GameService::RunGame(std::shared_ptr<Model::GameModel>& currentGame, const std::string& nickName)
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

bool GameService::IronHand(std::shared_ptr<Model::GameModel>& currentGame)
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

GameDTO GameService::ToGameDTO(std::shared_ptr<Model::GameModel> gameModel)
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

std::shared_ptr<Model::GameModel> GameService::ToGameModel(Repository::DTOs::GameDTO gameDTO)
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

void GameService::DistributeCards(std::shared_ptr<Model::GameModel>& currentGame)
{
    vector<int> cardsRemoved;
    int cardsDealt = 0;
    int countCards = 0;
    for (auto& player : currentGame->GetPlayers())
    {
        cardsDealt = 0;
        for (auto& card : currentGame->GetGameCardDeck()->GetCardDeck())
        {
            if (cardsDealt > 3)
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

void GameService::TurnCard(std::shared_ptr<Model::GameModel>& currentGame)
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


    // Acho que esse trecho de código não é necessário, verificar na validação ------
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

void Service::GameService::ConnectGameAsHost(std::shared_ptr<Model::GameModel>& currentGame, std::shared_ptr<Model::UserModel>& currentUser)
{
    StructMessage receivedConfirmation = _communicationService->ReceiveMessageHost();
    if (!receivedConfirmation.MessageSuccessfuly)
    {
        throw Exceptions::GameInvalid("O jogador no conseguiu se conectar na partida!");
    }

    StartGame(currentGame);
    Hand(currentGame);

    StructMessage gameMessage;
    gameMessage.MessageSuccessfuly = true;
    gameMessage.Content = Serialize::ConvertGameModelToString(currentGame);
    _communicationService->SendMessageAsHost(gameMessage);
        
    StructMessage receivedMessage;
    while (true)
    {
        receivedMessage = _communicationService->ReceiveMessageHost();
    }
}

void Service::GameService::ConnectGameAsClient(std::shared_ptr<Model::GameModel>& currentGame, std::shared_ptr<Model::UserModel>& currentUser)
{
    StructMessage message;
    message.MessageSuccessfuly = true;
    message.Content = "Successfuly Client Connect?";
    bool messageSent = _communicationService->SendMessageAsClient(message);

    if (!messageSent)
    {
        throw Exceptions::GameInvalid("O host nao conseguiu inicializar a partida!");
    }

    StructMessage receivedGameMessage;
    receivedGameMessage = _communicationService->ReceiveMessageClient();
    if (receivedGameMessage.MessageSuccessfuly)
        currentGame->CopyFrom(Serialize::ConvertStringToGameModel(receivedGameMessage.Content));

    /*StructMessage receivedMessage;
    while (true)
    {
        receivedMessage = _communicationService->ReceiveMessageClient();
    }*/
}

void Service::GameService::WaitTurn()
{
}
