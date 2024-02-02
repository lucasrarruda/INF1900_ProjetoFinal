#pragma once
#include <string>
#include <Repository/DTOs/BaseDTO.h>
#include <Helpers/Enums/ModeGame.h>
#include <memory>
#include <CardDeckModel.h>
#include <PlayerModel.h>

namespace Repository::DTOs
{
    class GameDTO : public BaseDTO
    {
    public:
        GameDTO() = default;
        GameDTO(const bool& playGame, const int& turnPlayer, const Helpers::Enums::ModeGameEnum& modeGame, 
            const std::shared_ptr<Model::CardDeckModel>& gameCardDeck, const std::map<int, std::shared_ptr<Model::PlayerModel>>& players, const int& handPoints,
            const bool& firstRound, const int& playerOneDiscardCardKey, const int& playerTwoDiscardCardKey, 
            const int& playerThreeDiscardCardKey, const int& playerFourDiscardCardKey) : PlayGame(playGame), TurnPlayer(turnPlayer), ModeGame(modeGame), 
            GameCardDeck(gameCardDeck), Players(players), HandPoints(handPoints), FirstRound(firstRound), PlayerOneDiscardCardKey(playerOneDiscardCardKey), 
            PlayerTwoDiscardCardKey(playerTwoDiscardCardKey), PlayerThreeDiscardCardKey(playerThreeDiscardCardKey), PlayerFourDiscardCardKey(playerFourDiscardCardKey) {}
        ~GameDTO() = default;

        bool PlayGame;
        int TurnPlayer;
        Helpers::Enums::ModeGameEnum ModeGame;
        std::shared_ptr<Model::CardDeckModel> GameCardDeck;
        std::map<int, std::shared_ptr<Model::PlayerModel>> Players;
        int HandPoints;
        bool FirstRound;
        int PlayerOneDiscardCardKey;
        int PlayerTwoDiscardCardKey;
        int PlayerThreeDiscardCardKey;
        int PlayerFourDiscardCardKey;
    };
}