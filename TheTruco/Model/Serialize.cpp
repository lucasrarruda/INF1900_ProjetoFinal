#include "pch.h"
#define stringify( name ) # name
#include <Windows.h>
#include <Serialize.h>
#include <Helpers/DatabaseUtils.h>
#include <Helpers/Utils.h>
#include <filesystem>
#include <stack>
#include <stringapiset.h>
#include <sstream>
#include <guiddef.h>

using namespace Helpers;
using namespace std;
using namespace Repository::DTOs;
using namespace Model;
using namespace Helpers::Enums;

const string modeGameEnumString[] =
{
	stringify(ModeGameEnum::INDIVIDUAL),
	stringify(ModeGameEnum::PAIR)
};

UserDTO Serialize::ConvertStringToUserDTO(const std::string& result)
{
    vector<string> response = Utils::SplitString(result, ";");
    UserDTO userDTO;

    int count = 0;
    for (auto& value : response)
    {
        count++;
        switch (count)
        {
        case 1:
            userDTO.Id = ConvertStringToGUID(value);
            continue;
        case 2:
            userDTO.NickName = value;
            continue;
        case 3:
            userDTO.GamesWin = stoi(value);
            continue;
        case 4:
            userDTO.GamesLose = stoi(value);
            continue;
		case 5:
			userDTO.CurrentGameID = value;
			continue;
		case 6:
			userDTO.OnCurrentGame = (value.compare("true") == 0) ? true : false;
			continue;
        }
    }

    return userDTO;
}

string Serialize::ConvertUserDTOToString(const UserDTO& result)
{
    string GUID = ConvertGUIDToString(result.Id);
    string nickName = result.NickName;
    string gamesWin = to_string(result.GamesWin);
    string gamesLose = to_string(result.GamesLose);
	string currentGameID = result.CurrentGameID;
	string onCurrentGame = result.OnCurrentGame ? "true" : "false";

    string resultString = GUID + ";" + nickName + ";" + gamesWin + ";" + gamesLose + ";" + currentGameID + ";" + onCurrentGame;

    return resultString;
}

GameDTO Serialize::ConvertStringToGameDTO(const std::string& result)
{
	vector<string> response = Utils::SplitString(result, ";");
	GameDTO gameDTO;

	Helpers::Enums::ModeGameEnum ModeGame;
	std::shared_ptr<Model::CardDeckModel> GameCardDeck;
	std::map<int, std::shared_ptr<Model::PlayerModel>> Players;
	wstring gameCardDeck = L"";
	wstring players = L"";

	int count = 0;
	for (auto& value : response)
	{
		count++;
		switch (count)
		{
		case 1:
			gameDTO.Id = ConvertStringToGUID(value);
			continue;
		case 2:
			gameDTO.PlayGame = (value.compare("true") == 0) ? true : false;
			continue;
		case 3:
			gameDTO.TurnPlayer = stoi(value);
			continue;
		case 4:
			if (value.compare(modeGameEnumString[ModeGameEnum::INDIVIDUAL]) == 0)
			{
				gameDTO.ModeGame = ModeGameEnum::INDIVIDUAL;
			}
			else
			{
				gameDTO.ModeGame = ModeGameEnum::PAIR;
			}
			continue;
		case 5:
			if (gameCardDeck.compare(L"") == 0)
			{
				DatabaseUtils::Get(ConvertStringToWString(value), L"GameCardDeck", gameCardDeck);

				gameDTO.GameCardDeck = ConvertStringToGameCardDeckModel(ConvertWStringToString(gameCardDeck));
			}
			continue;
		case 6:
			if (players.compare(L"") == 0)
			{
				DatabaseUtils::Get(ConvertStringToWString(value), L"Players", players);

				gameDTO.Players = ConvertStringToPlayerModelMap(ConvertWStringToString(players));
			}
			continue;
		case 7:
			gameDTO.HandPoints = stoi(value);
			continue;
		case 8:
			gameDTO.FirstRound = (value.compare("true") == 0) ? true : false;
			continue;
		case 9:
			gameDTO.PlayerOneDiscardCardKey = stoi(value);
			continue;
		case 10:
			gameDTO.PlayerTwoDiscardCardKey = stoi(value);
			continue;
		case 11:
			gameDTO.PlayerThreeDiscardCardKey = stoi(value);
			continue;
		case 12:
			gameDTO.PlayerFourDiscardCardKey = stoi(value);
			continue;
		}
	}

	return gameDTO;
}

string Serialize::ConvertGameDTOToString(const GameDTO& result)
{
	string GUID = ConvertGUIDToString(result.Id);
	string playGame = result.PlayGame ? "true" : "false";
	string turnPlayer = to_string(result.TurnPlayer);
	string modeGame = modeGameEnumString[result.ModeGame];
	string gameCardDeck = GUID;
	string players = GUID;
	string handPoints = to_string(result.HandPoints);
	string firstRound = result.FirstRound ? "true" : "false";
	string playerOneDiscardCardKey = to_string(result.PlayerOneDiscardCardKey);
	string playerTwoDiscardCardKey = to_string(result.PlayerTwoDiscardCardKey);
	string playerThreeDiscardCardKey = to_string(result.PlayerThreeDiscardCardKey);
	string playerFourDiscardCardKey = to_string(result.PlayerFourDiscardCardKey);

	if (result.GameCardDeck != nullptr)
	{
		DatabaseUtils::Set(ConvertStringToWString(GUID), L"GameCardDeck", ConvertStringToWString(ConvertGameCardDeckModelToString(result.GameCardDeck)));
	}

	DatabaseUtils::Set(ConvertStringToWString(GUID), L"Players", ConvertStringToWString(ConvertPlayerModelMapToString(result.Players, GUID)));

	string resultString = GUID + ";" + playGame + ";" + turnPlayer + ";" + modeGame + ";" + gameCardDeck + ";" + players + ";" + handPoints + ";" + firstRound + ";" 
		+ playerOneDiscardCardKey + ";" + playerTwoDiscardCardKey + ";" + playerThreeDiscardCardKey + ";" + playerFourDiscardCardKey;

	return resultString;
}

shared_ptr<GameModel> Serialize::ConvertStringToGameModel(const string& result)
{
	vector<string> response = Utils::SplitString(result, ";");
	shared_ptr<GameModel> gameModel = make_shared<GameModel>();

	Helpers::Enums::ModeGameEnum ModeGame;
	std::map<int, std::shared_ptr<Model::PlayerModel>> Players;
	wstring gameCardDeck = L"";
	wstring players = L"";

	int count = 0;
	for (auto& value : response)
	{
		count++;
		switch (count)
		{
		case 1:
			gameModel->SetId(value);
			continue;
		case 2:
			gameModel->SetPlayGame((value.compare("true") == 0) ? true : false);
			continue;
		case 3:
			gameModel->SetTurnPlayer(stoi(value));
			continue;
		case 4:
			if (value.compare(modeGameEnumString[ModeGameEnum::INDIVIDUAL]) == 0)
			{
				gameModel->SetModeGame(ModeGameEnum::INDIVIDUAL);
			}
			else
			{
				gameModel->SetModeGame(ModeGameEnum::PAIR);
			}
			continue;
		case 5:
			if (gameCardDeck.compare(L"") == 0)
			{
				DatabaseUtils::Get(ConvertStringToWString(value), L"GameCardDeckCopy", gameCardDeck);

				gameModel->SetGameCardDeck(ConvertStringToGameCardDeckModel(ConvertWStringToString(gameCardDeck)));
			}
			continue;
		case 6:
			if (players.compare(L"") == 0)
			{
				DatabaseUtils::Get(ConvertStringToWString(value), L"PlayersCopy", players);

				gameModel->SetPlayers(ConvertStringToPlayerModelMap(ConvertWStringToString(players)));
			}
			continue;
		case 7:
			gameModel->SetHandPoints(stoi(value));
			continue;
		case 8:
			gameModel->SetFirstRound((value.compare("true") == 0) ? true : false);
			continue;
		case 9:
			gameModel->SetPlayerOneDiscardCardKey(stoi(value));
			continue;
		case 10:
			gameModel->SetPlayerTwoDiscardCardKey(stoi(value));
			continue;
		case 11:
			gameModel->SetPlayerThreeDiscardCardKey(stoi(value));
			continue;
		case 12:
			gameModel->SetPlayerFourDiscardCardKey(stoi(value));
			continue;
		case 13:
			gameModel->SetLeaveGame((value.compare("true") == 0) ? true : false);
			continue;
		}
	}

	return gameModel;
}

string Serialize::ConvertGameModelToString(const shared_ptr<GameModel>& result)
{
	string GUID = result->GetId();
	string playGame = result->GetPlayGame() ? "true" : "false";
	string turnPlayer = to_string(result->GetTurnPlayer());
	string modeGame = modeGameEnumString[result->GetModeGame()];
	string gameCardDeck = GUID;
	string players = GUID;
	string handPoints = to_string(result->GetHandPoints());
	string firstRound = result->GetFirstRound() ? "true" : "false";
	string playerOneDiscardCardKey = to_string(result->GetPlayerOneDiscardCardKey());
	string playerTwoDiscardCardKey = to_string(result->GetPlayerTwoDiscardCardKey());
	string playerThreeDiscardCardKey = to_string(result->GetPlayerThreeDiscardCardKey());
	string playerFourDiscardCardKey = to_string(result->GetPlayerFourDiscardCardKey());
	string leaveGame = result->GetLeaveGame() ? "true" : "false";

	if (result->GetGameCardDeck() != nullptr)
	{
		DatabaseUtils::Set(ConvertStringToWString(GUID), L"GameCardDeckCopy", ConvertStringToWString(ConvertGameCardDeckModelToString(result->GetGameCardDeck())));
	}

	DatabaseUtils::Set(ConvertStringToWString(GUID), L"PlayersCopy", ConvertStringToWString(ConvertPlayerModelMapToString(result->GetPlayers(), GUID)));

	string resultString = GUID + ";" + playGame + ";" + turnPlayer + ";" + modeGame + ";" + gameCardDeck + ";" + players + ";" + handPoints + ";" + firstRound + ";"
		+ playerOneDiscardCardKey + ";" + playerTwoDiscardCardKey + ";" + playerThreeDiscardCardKey + ";" + playerFourDiscardCardKey + ";" + leaveGame;

	return resultString;
}

string Serialize::ConvertGameCardDeckModelToString(const shared_ptr<CardDeckModel>& cardDeckModel)
{
	return ConvertPlayingCardModelVectorToString(cardDeckModel->GetCardDeck());
}

shared_ptr<CardDeckModel> Serialize::ConvertStringToGameCardDeckModel(const string& cardDeckModel)
{
	if (cardDeckModel.empty())
		return shared_ptr<CardDeckModel>();

	vector<string> response = Utils::SplitString(cardDeckModel, "|");
	auto cardDeck = make_shared<CardDeckModel>();

	vector<pair<int, shared_ptr<PlayingCardModel>>> playingCards;

	vector<string> playingCardModelResponse;
	int count = 0;
	for (auto& value : response)
	{
		playingCardModelResponse = Utils::SplitString(value, ";");

		auto playingCardModel = std::make_shared<PlayingCardModel>(playingCardModelResponse[1], playingCardModelResponse[2]);
		playingCardModel->SetReferenceValueActual(stoi(playingCardModelResponse[3]));

		pair<int, shared_ptr<PlayingCardModel>> card;
		card.first = stoi(playingCardModelResponse[0]);
		card.second = playingCardModel;

		playingCards.insert(playingCards.begin() + count, card);
		count++;
	}

	cardDeck->SetCardDeck(playingCards);

	return cardDeck;
}

string Serialize::ConvertPlayerModelMapToString(const map<int, shared_ptr<PlayerModel>>& playerModelMap, const string& id)
{
	string playerModelKey;
	string playerModelValue;
	string resultString = "";

	string playingCardModelString = "";
	string ready;
	string hostPlayer;
	string isBot;

	for (auto& player : playerModelMap)
	{
		playerModelKey = to_string(player.first);
		ready = player.second->GetReady() ? "true" : "false";
		hostPlayer = player.second->IsHostPlayer() ? "true" : "false";
		isBot = player.second->GetIsBot() ? "true" : "false";

		auto playingCardModelVector = ConvertPlayingCardModelMapToPlayingCardModelVector(player.second->GetCardDeck());
		playingCardModelString = ConvertPlayingCardModelVectorToString(playingCardModelVector);

		playerModelValue = player.second->GetNickName() + ";" + ready + ";" + to_string(player.second->GetNumberPlayer()) + ";" + hostPlayer + ";" +
			to_string(player.second->GetHandPoints()) + ";" + isBot + ";" + id + ";" + to_string(player.second->GetRoundPoints());

		DatabaseUtils::Set(ConvertStringToWString(id), ConvertStringToWString(player.second->GetNickName()) + L"Players-PlayingCards", ConvertStringToWString(playingCardModelString));

		if (resultString.compare("") == 0)
		{
			resultString = playerModelKey + ";" + playerModelValue;
		}
		else
		{
			resultString = resultString + "|" + playerModelKey + ";" + playerModelValue;
		}
	}

	return resultString;
}

map<int, shared_ptr<PlayerModel>> Serialize::ConvertStringToPlayerModelMap(const string& playerModelMap)
{
	vector<string> response = Utils::SplitString(playerModelMap, "|");
	auto playerMap = map<int, shared_ptr<PlayerModel>>();

	bool ready;
	bool hostPlayer;
	bool isBot;
	wstring playingCardModelString;

	vector<string> playerModelResponse;
	for (auto& value : response)
	{
		playerModelResponse = Utils::SplitString(value, ";");

		ready = (playerModelResponse[2].compare("true") == 0) ? true : false;
		hostPlayer = (playerModelResponse[4].compare("true") == 0) ? true : false;
		isBot = (playerModelResponse[6].compare("true") == 0) ? true : false;

		auto player = std::make_shared<PlayerModel>(playerModelResponse[1], ready, stoi(playerModelResponse[3]), hostPlayer);
		player->SetHandPoints(stoi(playerModelResponse[5]));
		player->SetIsBot(isBot);
		player->SetRoundPoints(stoi(playerModelResponse[8]));

		DatabaseUtils::Get(ConvertStringToWString(playerModelResponse[7]), ConvertStringToWString(playerModelResponse[1]) + L"Players-PlayingCards", playingCardModelString);

		auto playingCardModelVector = ConvertStringToPlayingCardModelVector(ConvertWStringToString(playingCardModelString));

		std::map<int, std::shared_ptr<PlayingCardModel>> playingCardModelmap(playingCardModelVector.begin(), playingCardModelVector.end());
		player->SetCardDeck(playingCardModelmap);

		playerMap.try_emplace(stoi(playerModelResponse[0]), player);
	}

	return playerMap;
}

string Serialize::ConvertPlayingCardModelVectorToString(const vector<pair<int, shared_ptr<PlayingCardModel>>>& playingCardModelVector)
{
	string cardDeckKey;
	string cardDeckValue;
	string resultString = "";

	for (auto& value : playingCardModelVector)
	{
		cardDeckKey = to_string(value.first);
		cardDeckValue = value.second->GetSuit() + ";" + value.second->GetRealValue() + ";" + to_string(value.second->GetReferenceValueActual());

		if (resultString.compare("") == 0)
		{
			resultString = cardDeckKey + ";" + cardDeckValue;
		}
		else
		{
			resultString = resultString + "|" + cardDeckKey + ";" + cardDeckValue;
		}
	}

	return resultString;
}

vector<pair<int, shared_ptr<PlayingCardModel>>> Serialize::ConvertStringToPlayingCardModelVector(const string& playingCardModelVector)
{
	if (playingCardModelVector.empty())
		return vector<pair<int, shared_ptr<PlayingCardModel>>>();

	vector<string> response = Utils::SplitString(playingCardModelVector, "|");
	auto playingCardVector = vector<pair<int, shared_ptr<PlayingCardModel>>>();

	vector<string> playingCardModelResponse;
	int count = 0;
	for (auto& value : response)
	{
		playingCardModelResponse = Utils::SplitString(value, ";");

		auto playingCard = std::make_shared<PlayingCardModel>(playingCardModelResponse[1], playingCardModelResponse[2]);
		playingCard->SetReferenceValueActual(stoi(playingCardModelResponse[3]));

		pair<int, shared_ptr<PlayingCardModel>> card;
		card.first = stoi(playingCardModelResponse[0]);
		card.second = playingCard;

		playingCardVector.insert(playingCardVector.begin() + count, card);
		count++;
	}

	return playingCardVector;
}

vector<pair<int, shared_ptr<PlayingCardModel>>> Serialize::ConvertPlayingCardModelMapToPlayingCardModelVector(const map<int, shared_ptr<PlayingCardModel>>& playingCardModelMap)
{
	vector<pair<int, shared_ptr<PlayingCardModel>>> playingCardDeckVector;

	int count = 0;
	for (auto iterator = playingCardModelMap.begin(); iterator != playingCardModelMap.end(); ++iterator)
	{
		pair<int, shared_ptr<PlayingCardModel>> card;
		card.first = iterator->first;
		card.second = iterator->second;

		playingCardDeckVector.insert(playingCardDeckVector.begin() + count, card);
		count++;
	}

	return playingCardDeckVector;
}


wstring Serialize::ConvertStringToWString(const std::string& object)
{
    if (object.empty())
        return wstring();

    int size = MultiByteToWideChar(CP_UTF8, 0, &object[0], static_cast<int>(object.size()), 0, 0);

    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &object[0], static_cast<int>(object.size()), &result[0], size);

    return result;
}

string Serialize::ConvertWStringToString(const std::wstring& object)
{
    if (object.empty())
        return string();

    int size = WideCharToMultiByte(CP_UTF8, 0, &object[0], static_cast<int>(object.size()), 0, 0, 0, 0);

    string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &object[0], static_cast<int>(object.size()), &result[0], size, 0, 0);

    return result;
}

GUID Serialize::ConvertStringToGUID(const std::string id)
{
    CLSID clsID;
    auto temporary = wstring(id.begin(), id.end());
    auto wideString = temporary.c_str();
    CLSIDFromString(wideString, &clsID);

    return clsID;
}

std::string Serialize::ConvertGUIDToString(GUID Id)
{
    wchar_t guid_wchar[40];
    StringFromGUID2(Id, guid_wchar, 40);

    char guid_char[40];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP, 0, guid_wchar, -1, guid_char, 260, &DefChar, nullptr);

    return guid_char;
}
