#pragma once
#include <string>
#include <map>
#include <Helpers/Constants/CardsValueConstants.h>
#include <Helpers/Enums/CardsValueEnum.h>
#include <Helpers/Constants/SuitsConstants.h>
#include <Helpers/Enums/SuitsEnum.h>
#include <Interfaces/ModelBase.h>

namespace Model
{
    class PlayingCardModel : public Interfaces::ModelBase
    {
    public:
        PlayingCardModel() = default;
        PlayingCardModel(const std::string& suit, const std::string& cardValue);
        ~PlayingCardModel() = default;

        inline std::string GetSuit() const { return _suit; }

        inline std::string GetRealValue() const { return _cardValue; }

        inline int GetReferenceValueActual() const { return _referenceValueActual; }
        
        inline int GetReferenceValue() const { return _referenceValue; }
        inline void SetReferenceValueActual(const int& referenceValueActual) { _referenceValueActual = referenceValueActual; };
        void SetReferenceValueActual(const bool& manila);

    private:
        std::string _suit = "";
        std::string _cardValue = "";
        int _referenceValue = 0;
        int _referenceValueActual = 0;
        
        std::map<std::string, int> _cardsValues
        {
            { Helpers::CardsValueConstants::THREE, static_cast<int>(Helpers::Enums::CardsValueEnum::THREE) },
            { Helpers::CardsValueConstants::TWO, static_cast<int>(Helpers::Enums::CardsValueEnum::TWO) },
            { Helpers::CardsValueConstants::A, static_cast<int>(Helpers::Enums::CardsValueEnum::A) },
            { Helpers::CardsValueConstants::K, static_cast<int>(Helpers::Enums::CardsValueEnum::K) },
            { Helpers::CardsValueConstants::J, static_cast<int>(Helpers::Enums::CardsValueEnum::J) },
            { Helpers::CardsValueConstants::Q, static_cast<int>(Helpers::Enums::CardsValueEnum::Q) },
            { Helpers::CardsValueConstants::SEVEN, static_cast<int>(Helpers::Enums::CardsValueEnum::SEVEN) },
            { Helpers::CardsValueConstants::SIX, static_cast<int>(Helpers::Enums::CardsValueEnum::SIX) },
            { Helpers::CardsValueConstants::FIVE, static_cast<int>(Helpers::Enums::CardsValueEnum::FIVE) },
            { Helpers::CardsValueConstants::FOUR, static_cast<int>(Helpers::Enums::CardsValueEnum::FOUR) }
        };

        std::map<std::string, int> _suits
        {
            { Helpers::SuitsConstants::DIAMONDS, static_cast<int>(Helpers::Enums::SuitsEnum::DIAMONDS) },
            { Helpers::SuitsConstants::SPADES, static_cast<int>(Helpers::Enums::SuitsEnum::SPADES) },
            { Helpers::SuitsConstants::HEARTS, static_cast<int>(Helpers::Enums::SuitsEnum::HEARTS) },
            { Helpers::SuitsConstants::CLUBS, static_cast<int>(Helpers::Enums::SuitsEnum::CLUBS) }
        };

        void SetReferenceValue();
    };
}