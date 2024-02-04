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

vector<shared_ptr<UserModel>> UserService::GetAllUsers()
{
    vector<UserDTO> usersDTO = _userRepository.GetAll(DATABASE_USERS);

    vector<shared_ptr<UserModel>> users;
    int count = 0;
	for (auto& user : usersDTO)
	{
        users.insert(users.begin() + count, ToUserModel(user));
        count++;
	}

    return users;
}

shared_ptr<Model::UserModel> UserService::GetUserById(const string& id)
{
    UserDTO user = _userRepository.GetById(DATABASE_USERS, id);

    return ToUserModel(user);
}

shared_ptr<Model::UserModel> UserService::SaveUser(shared_ptr<Model::UserModel> user)
{
    UserDTO userNew;

    userNew = ToUserDTO(
        make_shared<UserModel>(
            user->GetId(), 
            user->GetNickName(), 
            user->GetGamesWin(), 
            user->GetGamesLose(), 
            user->GetCurrentGameID(), 
            user->GetOnCurrentGame()));
    return ToUserModel(_userRepository.Save(DATABASE_USERS, userNew.NickName, userNew));
}

void UserService::UpdateUser(shared_ptr<Model::UserModel> user)
{
    UserDTO userNew;

    if (user->GetId().compare("") != 0)
    {
        userNew = _userRepository.GetById(DATABASE_USERS, user->GetId());
    }

    if (userNew.NickName.compare("") != 0)
    {
        userNew.NickName = user->GetNickName();
        userNew.GamesWin = user->GetGamesWin();
        userNew.GamesLose = user->GetGamesLose();
        userNew.CurrentGameID = user->GetCurrentGameID();
        userNew.OnCurrentGame = user->GetOnCurrentGame();

        _userRepository.Update(DATABASE_USERS, userNew.NickName, userNew);
    }
}

void UserService::RemoveUser(shared_ptr<Model::UserModel> user)
{
    auto userById = _userRepository.GetById(DATABASE_USERS, user->GetId());
    
    if (userById.NickName.compare("") != 0)
    {
        _userRepository.Remove(DATABASE_USERS, userById.NickName);
    }
}

shared_ptr<Model::UserModel> UserService::GetConflictingUser(shared_ptr<Model::UserModel> user)
{
    UserDTO userConflictngDTO = _userRepository.GetConflictingUser(ToUserDTO(user));

    if (userConflictngDTO.NickName.compare("") == 0)
        return make_shared<UserModel>();

    shared_ptr<UserModel> userConflictng = ToUserModel(userConflictngDTO);

    return userConflictng;
}

UserDTO UserService::ToUserDTO(shared_ptr<Model::UserModel> userModel)
{
    return UserDTO(
        userModel->GetNickName(), 
        userModel->GetGamesWin(), 
        userModel->GetGamesLose(), 
        userModel->GetCurrentGameID(), 
        userModel->GetOnCurrentGame());
}

shared_ptr<Model::UserModel> UserService::ToUserModel(UserDTO userDTO)
{
    return make_shared<UserModel>(
        Serialize::ConvertGUIDToString(userDTO.Id), 
        userDTO.NickName, 
        userDTO.GamesWin, 
        userDTO.GamesLose, 
        userDTO.CurrentGameID, 
        userDTO.OnCurrentGame);
}
