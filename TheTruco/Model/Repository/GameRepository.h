#pragma once
#include <string>
#include <Repository/Base/BaseRepository.h>
#include <Repository/DTOs/GameDTO.h>

namespace Repository
{
    class GameRepository : public Repository::Base::BaseRepository<DTOs::GameDTO>
    {
    public:
        DTOs::GameDTO ConvertStringToModel(const std::string& result) override;
        std::string ConvertModelToString(const DTOs::GameDTO& result) override;

        void Remove(const std::string& section, const std::string& key) override;
        DTOs::GameDTO GetConflictingGame(const DTOs::GameDTO& gameDTO);
    };
}