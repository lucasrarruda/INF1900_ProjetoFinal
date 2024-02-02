#pragma once
#include <string>
#include <Repository/UserRepository.h>
#include <UserModel.h>

namespace Service
{
    class UserService
    {
    public:
        UserService() = default;
        explicit UserService(const Repository::UserRepository& userRepository) : _userRepository(userRepository) {};
        ~UserService() = default;

        std::vector<Model::UserModel> GetAllUsers();
        Model::UserModel GetUserById(const std::string& id);
        Model::UserModel SaveUser(const Model::UserModel& user);
        void UpdateUser(const Model::UserModel& user);
        void RemoveUser(const Model::UserModel& user);
        Model::UserModel GetConflictingUser(Model::UserModel user);

    private: 
        Repository::UserRepository _userRepository;

        Repository::DTOs::UserDTO ToUserDTO(Model::UserModel userModel);
        Model::UserModel ToUserModel(Repository::DTOs::UserDTO userDTO);
    };
}