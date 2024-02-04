#pragma once
#include <string>
#include <memory>
#include <PlayerModel.h>
#include <Helpers/Enums/ModeGame.h>
#include <CardDeckModel.h>
#include <Interfaces/ModelBase.h>

namespace Model
{
    class GameModel : public Interfaces::ModelBase
    {
    public:
        GameModel() = default;
        GameModel(const std::string& id, const Helpers::Enums::ModeGameEnum& modeGame)
            : _id(id), _modeGame(modeGame) {}
        GameModel(const std::string& id, const bool& playGame, const int& turnPlayer, const Helpers::Enums::ModeGameEnum& modeGame, 
            const std::shared_ptr<Model::CardDeckModel>& gameCardDeck, const std::map<int, std::shared_ptr<PlayerModel>>& players, 
            const int& handPoints, const bool& firstRound, const int& playerOneDiscardCardKey, const int& playerTwoDiscardCardKey, 
            const int& playerThreeDiscardCardKey, const int& playerFourDiscardCardKey) : _id(id), _playGame(playGame), _turnPlayer(turnPlayer), _modeGame(modeGame), 
            _gameCardDeck(gameCardDeck), _players(players), _handPoints(handPoints), _firstRound(firstRound), _playerOneDiscardCardKey(playerOneDiscardCardKey), _playerTwoDiscardCardKey(playerTwoDiscardCardKey), 
            _playerThreeDiscardCardKey(playerThreeDiscardCardKey), _playerFourDiscardCardKey(playerFourDiscardCardKey) {}
        ~GameModel() = default;

        inline std::string GetId() const { return _id; }
        inline void SetId(const std::string& id) { _id = id; };

        inline bool GetPlayGame() const { return _playGame; }
        inline void SetPlayGame(const bool& playGame) { _playGame = playGame; };

        inline int GetTurnPlayer() const { return _turnPlayer; }
        inline void SetTurnPlayer(const int& turnPlayer) { _turnPlayer = turnPlayer; };

        inline std::shared_ptr<Model::CardDeckModel> GetGameCardDeck() const { return _gameCardDeck; }
        inline void SetGameCardDeck(const std::shared_ptr<Model::CardDeckModel>& gameCardDeck) { _gameCardDeck = gameCardDeck; };

        inline std::map<int, std::shared_ptr<PlayerModel>> GetPlayers() const { return _players; }

        inline Helpers::Enums::ModeGameEnum GetModeGame() const { return _modeGame; }
        inline void SetModeGame(const Helpers::Enums::ModeGameEnum& modeGame) { _modeGame = modeGame; };

        inline int GetHandPoints() const { return _handPoints; }
        inline void SetHandPoints(const int& handPoints) { _handPoints = handPoints; };
        
        inline bool GetFirstRound() const { return _firstRound; }
        inline void SetFirstRound(const bool& firstRound) { _firstRound = firstRound; };

        inline int GetPlayerOneDiscardCardKey() const { return _playerOneDiscardCardKey; }
        inline void SetPlayerOneDiscardCardKey(const int& playerOneDiscardCardKey) { _playerOneDiscardCardKey = playerOneDiscardCardKey; };
        
        inline int GetPlayerTwoDiscardCardKey() const { return _playerTwoDiscardCardKey; }
        inline void SetPlayerTwoDiscardCardKey(const int& playerTwoDiscardCardKey) { _playerTwoDiscardCardKey = playerTwoDiscardCardKey; };
        
        inline int GetPlayerThreeDiscardCardKey() const { return _playerThreeDiscardCardKey; }
        inline void SetPlayerThreeDiscardCardKey(const int& playerThreeDiscardCardKey) { _playerThreeDiscardCardKey = playerThreeDiscardCardKey; };
        
        inline int GetPlayerFourDiscardCardKey() const { return _playerFourDiscardCardKey; }
        inline void SetPlayerFourDiscardCardKey(const int& playerFourDiscardCardKey) { _playerFourDiscardCardKey = playerFourDiscardCardKey; };

        void AddPlayerToGame(const std::shared_ptr<PlayerModel>& player);
        void RemovePlayerFromGame(const int& playerkey);

        inline void CopyFrom(const std::shared_ptr<GameModel> other)
        {
            if (this != other.get())
            {
                _id = other->_id;
                _playGame = other->_playGame;
                _turnPlayer = other->_turnPlayer;
                _modeGame = other->_modeGame;
                _handPoints = other->_handPoints;
                _firstRound = other->_firstRound;
                _playerOneDiscardCardKey = other->_playerOneDiscardCardKey;
                _playerTwoDiscardCardKey = other->_playerTwoDiscardCardKey;
                _playerThreeDiscardCardKey = other->_playerThreeDiscardCardKey;
                _playerFourDiscardCardKey = other->_playerFourDiscardCardKey;
                _gameCardDeck = other->_gameCardDeck;
                _players = other->_players;
            }
        }

    private:
        std::string _id = "";
        bool _playGame = false;
        int _turnPlayer = 1;
        Helpers::Enums::ModeGameEnum _modeGame;
        
        int _handPoints = 1;
        bool _firstRound = true;
        int _playerOneDiscardCardKey = 0;
        int _playerTwoDiscardCardKey = 0;
        int _playerThreeDiscardCardKey = 0;
        int _playerFourDiscardCardKey = 0;

        std::shared_ptr<Model::CardDeckModel> _gameCardDeck;
        std::map<int, std::shared_ptr<PlayerModel>> _players;
    };
}