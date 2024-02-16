#include "pch.h"
#include<iostream>
#include <random>
#include <CardDeckModel.h>
#include <Helpers/Enums/CardsValueEnum.h>
#include <Helpers/Enums/SuitsEnum.h>
#include <Helpers/Utils.h>

using namespace Model;
using namespace Helpers;
using namespace Helpers::Enums;
using namespace std;


void CardDeckModel::CreateCardDeck()
{
	int count = 0;
	for (int initCard = static_cast<int>(CardsValueEnum::FOUR); initCard <= static_cast<int>(CardsValueEnum::THREE); ++initCard)
	{
		for (int initSuit = static_cast<int>(SuitsEnum::DIAMONDS); initSuit <= static_cast<int>(SuitsEnum::CLUBS); ++initSuit)
		{
			auto playingCardModel = std::make_shared<PlayingCardModel>(Utils::GetSuitValue(initSuit), Utils::GetCardValue(initCard));
			
			pair<int, std::shared_ptr<Model::PlayingCardModel>> card;
			card.first = count;
			card.second = playingCardModel;
			_cardDeck.insert(_cardDeck.begin() + count, card);
			count++;
		}
	}
}

void CardDeckModel::RandomizeCardDeck()
{
	shuffle(_cardDeck.begin(), _cardDeck.end(), mt19937{random_device{}()});		
}

void CardDeckModel::ResetCardDeck()
{
	_cardDeck.clear();
	CreateCardDeck();
	RandomizeCardDeck();
}

void CardDeckModel::RemoveCardFromCardDeck(const int& cardKey)
{
	for (auto& card : _cardDeck)
	{
		if (card.first == cardKey)
		{
			auto it = std::find(_cardDeck.begin(), _cardDeck.end(), card);
			_cardDeck.erase(it);
			break;
		}
	}
}

void CardDeckModel::SetCardManila(const int& cardKey)
{
	for (auto& cardManila : _cardDeck)
	{
		if (cardManila.first == cardKey)
		{
			cardManila.second->SetReferenceValueActual(true);
			break;
		}
	}
}
