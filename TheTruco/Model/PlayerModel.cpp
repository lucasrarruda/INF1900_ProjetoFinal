#include "pch.h"
#include <PlayerModel.h>

using namespace Model;

void PlayerModel::AddCardToCardDeckPlayer(const std::shared_ptr<PlayingCardModel>& playingCard)
{
	_cardsDeck.try_emplace(static_cast<int>(_cardsDeck.size()), playingCard);
	Notify();
}

void PlayerModel::RemoveCardFromCardDeckPlayer(const int& cardkey)
{
	auto cardRemove = _cardsDeck.find(cardkey);
	_cardsDeck.erase(cardRemove);
}
