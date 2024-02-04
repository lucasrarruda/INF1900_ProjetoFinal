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

        std::vector<std::shared_ptr<Model::UserModel>> GetAllUsers();
        std::shared_ptr<Model::UserModel> GetUserById(const std::string& id);
        std::shared_ptr<Model::UserModel> SaveUser(std::shared_ptr<Model::UserModel> user);
        void UpdateUser(std::shared_ptr<Model::UserModel> user);
        void RemoveUser(std::shared_ptr<Model::UserModel> user);
        std::shared_ptr<Model::UserModel> GetConflictingUser(std::shared_ptr<Model::UserModel> user);

    private: 
        Repository::UserRepository _userRepository;

        Repository::DTOs::UserDTO ToUserDTO(std::shared_ptr<Model::UserModel> userModel);
        std::shared_ptr<Model::UserModel> ToUserModel(Repository::DTOs::UserDTO userDTO);
    };
}