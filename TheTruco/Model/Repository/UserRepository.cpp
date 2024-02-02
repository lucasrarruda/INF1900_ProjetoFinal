#include "pch.h"
#include <Repository/UserRepository.h>
#include <Serialize.h>
#include <Helpers/Constants/Constants.h>
#include <guiddef.h>

using namespace Repository::DTOs;
using namespace Repository;
using namespace Helpers;
using namespace Helpers::Constants;
using namespace std;

UserDTO UserRepository::ConvertStringToModel(const string& result)
{
    return Serialize::ConvertStringToUserDTO(result);
}

string UserRepository::ConvertModelToString(const UserDTO& result)
{
	return Serialize::ConvertUserDTOToString(result);
}

UserDTO UserRepository::GetConflictingUser(const UserDTO& userDTO)
{
    auto getUsers = GetAll(DATABASE_USERS);

    for (auto& user : getUsers)
    {
        if ((Serialize::ConvertGUIDToString(user.Id).compare(Serialize::ConvertGUIDToString(userDTO.Id)) == 0)
            && (user.NickName.compare(userDTO.NickName) == 0))
        {
            return user;
        }
    }

    return DTOs::UserDTO();
}
