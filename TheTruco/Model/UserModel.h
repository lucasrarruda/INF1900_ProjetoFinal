#pragma once
#include <string>
#include <Interfaces/ModelBase.h>

namespace Model
{
    class UserModel : public Interfaces::ModelBase
    {
    public:
        UserModel() = default;
        UserModel(const std::string& id, const std::string& nickName, const int& gamesWin, const int& gamesLose, const std::string& currentGameID, const bool& onCurrentGame)
            : _id(id), _nickName(nickName), _gamesWin(gamesWin), _gamesLose(gamesLose), _currentGameID(currentGameID), _onCurrentGame(onCurrentGame) {}
        ~UserModel() = default;

        inline std::string GetId() const { return _id; }

        inline std::string GetNickName() const { return _nickName; }
<<<<<<< HEAD
        inline void SetNickName(const std::string& nickname) { _nickName = nickname; }
=======
        inline void SetNickName(const std::string& nickName) { _nickName = nickName; };
>>>>>>> a1765d0a1e34f955603ef2e6b72d0383e356cbd7

        inline int GetGamesWin() const { return _gamesWin; }
        inline void SetGamesWin() { _gamesWin++; };

        inline int GetGamesLose() const { return _gamesLose; }
        inline void SetGamesLose() { _gamesLose++; };

        inline std::string GetCurrentGameID() const { return _currentGameID; }
        inline void SetCurrentGameID(const std::string& currentGameID) { _currentGameID = currentGameID; };

        inline bool GetOnCurrentGame() const { return _onCurrentGame; }
        inline void SetOnCurrentGame(const bool& onCurrentGame) { _onCurrentGame = onCurrentGame; };

    private:
        std::string _id = "";
        std::string _nickName = "";
        int _gamesWin = 0;
        int _gamesLose = 0;
        std::string _currentGameID = "";
        bool _onCurrentGame = false;
    };
}