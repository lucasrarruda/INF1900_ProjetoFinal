#include "pch.h"
#include <Repository/GameRepository.h>
#include <Helpers/Constants/Constants.h>
#include <Serialize.h>

using namespace Repository;
using namespace Repository::DTOs;
using namespace Helpers;
using namespace std;
using namespace Helpers::Constants;

GameDTO GameRepository::ConvertStringToModel(const std::string& result)
{
	return Serialize::ConvertStringToGameDTO(result);
}

string GameRepository::ConvertModelToString(const DTOs::GameDTO& result)
{

	return Serialize::ConvertGameDTOToString(result);
}

void GameRepository::Remove(const string& section, const string& key)
{
    std::wstring wsection = Helpers::Serialize::ConvertStringToWString(section);
    std::wstring wkey = Helpers::Serialize::ConvertStringToWString(key);

    Helpers::DatabaseUtils::Remove(wsection, wkey);
    Helpers::DatabaseUtils::Remove(wkey);
}

GameDTO GameRepository::GetConflictingGame(const GameDTO& gameDTO)
{
    auto getGames = GetAll(DATABASE_GAMES);

    for (auto& game : getGames)
    {
        if ((Serialize::ConvertGUIDToString(game.Id).compare(Serialize::ConvertGUIDToString(gameDTO.Id)) == 0))
        {
            return game;
        }
    }

    return DTOs::GameDTO();
}