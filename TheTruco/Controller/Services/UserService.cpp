#include "pch.h"
#include <Services/UserService.h>
#include <Serialize.h>
#include <Helpers/Constants/Constants.h>

using namespace Service;
using namespace Model;
using namespace Helpers;
using namespace Repository::DTOs;
using namespace std;
using namespace Helpers::Constants;

vector<UserModel> UserService::GetAllUsers()
{
    vector<UserDTO> usersDTO = _userRepository.GetAll(DATABASE_USERS);

    vector<UserModel> users;
    int count = 0;
	for (auto& user : usersDTO)
	{
        users.insert(users.begin() + count, ToUserModel(user));
        count++;
	}

    return users;
}

UserModel UserService::GetUserById(const string& id)
{
    UserDTO user = _userRepository.GetById(DATABASE_USERS, id);

    return ToUserModel(user);
}

Model::UserModel UserService::SaveUser(const UserModel& user)
{
    UserDTO userNew;

    userNew = ToUserDTO(UserModel(user.GetId(), user.GetNickName(), user.GetGamesWin(), user.GetGamesLose(), user.GetCurrentGameID(), user.GetOnCurrentGame()));
    return ToUserModel(_userRepository.Save(DATABASE_USERS, userNew.NickName, userNew));
}

void UserService::UpdateUser(const UserModel& user)
{
    UserDTO userNew;

    if (user.GetId().compare("") != 0)
    {
        userNew = _userRepository.GetById(DATABASE_USERS, user.GetId());
    }

    if (userNew.NickName.compare("") != 0)
    {
        userNew.NickName = user.GetNickName();
        userNew.GamesWin = user.GetGamesWin();
        userNew.GamesLose = user.GetGamesLose();

        _userRepository.Update(DATABASE_USERS, userNew.NickName, userNew);
    }
}

void UserService::RemoveUser(const UserModel& user)
{
    auto userByid = _userRepository.GetById(DATABASE_USERS, user.GetId());
    
    if (userByid.NickName.compare("") != 0)
    {
        _userRepository.Remove(DATABASE_USERS, userByid.NickName);
    }
}

UserModel UserService::GetConflictingUser(UserModel user)
{
    UserDTO userConflictngDTO = _userRepository.GetConflictingUser(ToUserDTO(user));

    if (userConflictngDTO.NickName.compare("") == 0)
        return UserModel();

    UserModel userConflictng = ToUserModel(userConflictngDTO);

    return userConflictng;
}

UserDTO UserService::ToUserDTO(UserModel userModel)
{
    return UserDTO(userModel.GetNickName(), userModel.GetGamesWin(), userModel.GetGamesLose(), userModel.GetCurrentGameID(), userModel.GetOnCurrentGame());
}

UserModel UserService::ToUserModel(UserDTO userDTO)
{
    return UserModel(Serialize::ConvertGUIDToString(userDTO.Id), userDTO.NickName, userDTO.GamesWin, userDTO.GamesLose, userDTO.CurrentGameID, userDTO.OnCurrentGame);
}
