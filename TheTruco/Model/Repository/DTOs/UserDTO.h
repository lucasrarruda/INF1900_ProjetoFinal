#pragma once
#include <string>
#include <Repository/DTOs/BaseDTO.h>

namespace Repository::DTOs
{
    class UserDTO : public BaseDTO
    {
    public:
        UserDTO() = default;
        UserDTO(const std::string& nickName, const int& gamesWin, const int& gamesLose, const std::string& currentGameID, const bool& onCurrentGame)
            : NickName(nickName), GamesWin(gamesWin), GamesLose(gamesLose), CurrentGameID(currentGameID), OnCurrentGame(onCurrentGame){}
        ~UserDTO() = default;

        std::string NickName;
        int GamesWin;
        int GamesLose;
        std::string CurrentGameID;
        bool OnCurrentGame;
    };
}