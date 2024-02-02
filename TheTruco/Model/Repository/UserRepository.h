#pragma once
#include <string>
#include <Repository/Base/BaseRepository.h>
#include <Repository/DTOs/UserDTO.h>

namespace Repository
{
    class UserRepository : public Repository::Base::BaseRepository<DTOs::UserDTO>
    {
    public:
        DTOs::UserDTO ConvertStringToModel(const std::string& result) override;
        std::string ConvertModelToString(const DTOs::UserDTO& result) override;

        DTOs::UserDTO GetConflictingUser(const DTOs::UserDTO& userDTO);
    };
}