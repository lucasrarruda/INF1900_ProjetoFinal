#include "pch.h"
#include <PlayingCardModel.h>

using namespace Model;

PlayingCardModel::PlayingCardModel(const std::string& suit, const std::string& cardValue) : _suit(suit), _cardValue(cardValue)
{
	SetReferenceValue();
	SetReferenceValueActual(false);
}

void PlayingCardModel::SetReferenceValueActual(const bool& manila)
{
	auto suit = _suits.find(_suit);
	if (suit != _suits.end())
	{
		if (manila)
		{
			_referenceValueActual = 100 * suit->second;
		}
		else
		{
			_referenceValueActual = _referenceValue;
		}
	}
}

void PlayingCardModel::SetReferenceValue()
{
	auto cardValue = _cardsValues.find(_cardValue);

	int cardValueReference = 0;
	
	if (cardValue != _cardsValues.end())
	{
		cardValueReference = cardValue->second;
		_referenceValue = cardValueReference;
	}
}
