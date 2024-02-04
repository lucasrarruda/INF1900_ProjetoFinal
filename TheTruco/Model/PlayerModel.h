#pragma once
#include <string>
#include <PlayingCardModel.h>
#include <memory>
#include <Interfaces/ModelBase.h>

namespace Model
{
    class PlayerModel : public Interfaces::ModelBase
    {
    public:
        PlayerModel() = default;
        PlayerModel(const std::string& nickName, const bool& ready, const int& numberPlayer, const bool& hostPlayer)
            : _nickName(nickName), _ready(ready), _numberPlayer(numberPlayer), _hostPlayer(hostPlayer) {}
        ~PlayerModel() = default;

        inline std::string GetNickName() const { return _nickName; }
        inline void SetNickName(const std::string& nickName) { _nickName = nickName; };

        inline bool GetReady() const { return _ready; }
        inline void SetReady(const bool& ready) { _ready = ready; };

        inline int GetNumberPlayer() const { return _numberPlayer; }
        inline void SetNumberPlayer(const int& numberPlayer) { _numberPlayer = numberPlayer; };

        inline bool IsHostPlayer() const { return _hostPlayer; }
        inline void SetHostPlayer(const bool& host) { _hostPlayer = host; };

        inline bool GetIsBot() const { return _isBot; }
        inline void SetIsBot(const bool& isBot) { _isBot = isBot; };

        inline int GetHandPoints() const { return _handPoints; }
        inline void SetHandPoints(const int& handPoints) { _handPoints = handPoints; };

        inline int GetRoundPoints() const { return _roundPoints; }
        inline void SetRoundPoints(const int& roundPoints) { _roundPoints = roundPoints; };

        inline std::map<int, std::shared_ptr<PlayingCardModel>> GetCardDeck() { return _cardsDeck; }
        inline void SetCardDeck(const std::map<int, std::shared_ptr<PlayingCardModel>>& cardsDeck) { _cardsDeck = cardsDeck; };

        void AddCardToCardDeckPlayer(const std::shared_ptr<PlayingCardModel>& playingCard);
        void RemoveCardFromCardDeckPlayer(const int& cardkey);

    private:
        std::string _nickName = "";
        bool _ready = false;
        int  _numberPlayer = 0;
        bool _hostPlayer = false;
        bool _isBot = false;
        
        int  _handPoints = 0;
        int  _roundPoints = 0;
        std::map<int, std::shared_ptr<PlayingCardModel>> _cardsDeck;
    };
}