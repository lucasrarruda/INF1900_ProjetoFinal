#include "pch.h"
#include <GameModel.h>

using namespace Model;

void GameModel::AddPlayerToGame(const std::shared_ptr<PlayerModel>& player)
{
	_players.try_emplace(static_cast<int>(_players.size()), player);
}

void GameModel::RemovePlayerFromGame(const int& playerkey)
{
	auto cardRemove = _players.find(playerkey);
	_players.erase(cardRemove);
}
