#pragma once
#include <vector>
#include <combaseapi.h>
#include <Repository/DTOs/UserDTO.h>
#include <Repository/DTOs/GameDTO.h>

namespace Helpers
{
    class Serialize
    {
    public:
        static Repository::DTOs::UserDTO ConvertStringToUserDTO(const std::string& result);
        static std::string ConvertUserDTOToString(const Repository::DTOs::UserDTO& result);

        static Repository::DTOs::GameDTO ConvertStringToGameDTO(const std::string& result);
        static std::string ConvertGameDTOToString(const Repository::DTOs::GameDTO& result);

        static std::string ConvertGameCardDeckModelToString(const std::shared_ptr<Model::CardDeckModel>& cardDeckModel);
        static std::shared_ptr<Model::CardDeckModel> ConvertStringToGameCardDeckModel(const std::string& cardDeckModel);

        static std::string ConvertPlayerModelMapToString(const  std::map<int, std::shared_ptr<Model::PlayerModel>>& playerModelMap, const std::string& id);
        static std::map<int, std::shared_ptr<Model::PlayerModel>> ConvertStringToPlayerModelMap(const std::string& playerModelMap);

        static std::string ConvertPlayingCardModelVectorToString(const  std::vector<std::pair<int, std::shared_ptr<Model::PlayingCardModel>>>& playingCardModelVector);
        static std::vector<std::pair<int, std::shared_ptr<Model::PlayingCardModel>>> ConvertStringToPlayingCardModelVector(const std::string& playingCardModelVector);

        static std::vector<std::pair<int, std::shared_ptr<Model::PlayingCardModel>>> ConvertPlayingCardModelMapToPlayingCardModelVector(const std::map<int, std::shared_ptr<Model::PlayingCardModel>>& playingCardModelMap);

        static std::wstring ConvertStringToWString(const std::string& object);
        static std::string ConvertWStringToString(const std::wstring& object);
        
        static GUID ConvertStringToGUID(const std::string id);
        static std::string ConvertGUIDToString(GUID Id);
    };
}