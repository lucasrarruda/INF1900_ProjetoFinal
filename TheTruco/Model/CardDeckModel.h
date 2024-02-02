#pragma once
#include <Interfaces/ModelBase.h>
#include <PlayingCardModel.h>
#include <memory>
#include<vector>

namespace Model
{
    class CardDeckModel : public Interfaces::ModelBase
    {
    public:
        CardDeckModel() = default;
        ~CardDeckModel() = default;

        inline std::vector<std::pair<int, std::shared_ptr<Model::PlayingCardModel>>> GetCardDeck() const { return _cardDeck; }
        inline void SetCardDeck(const std::vector<std::pair<int, std::shared_ptr<Model::PlayingCardModel>>>& cardDeck) { _cardDeck = cardDeck; };

        void CreateCardDeck();
        void RandomizeCardDeck();
        void ResetCardDeck();
        void RemoveCardFromCardDeck(const int& cardKey);
        void SetCardManila(const int& cardKey);

    private:
        std::vector<std::pair<int, std::shared_ptr<Model::PlayingCardModel>>> _cardDeck;
    };
}